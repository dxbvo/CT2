/* ------------------------------------------------------------------
 * --  _____       ______  _____                                    -
 * -- |_   _|     |  ____|/ ____|                                   -
 * --   | |  _ __ | |__  | (___    Institute of Embedded Systems    -
 * --   | | | '_ \|  __|  \___ \   Zuercher Hochschule Winterthur   -
 * --  _| |_| | | | |____ ____) |  (University of Applied Sciences) -
 * -- |_____|_| |_|______|_____/   8401 Winterthur, Switzerland     -
 * ------------------------------------------------------------------
 * --
 * -- Module      : SPI display mock
 * --
 * -- $Id: hal_mocked.c $
 * ------------------------------------------------------------------
 */

#ifdef MOCKED_SPI_DISPLAY

#include <string.h>
#include "hal_spi.h"
#include "hal_sbuf.h"
#include "reg_ctboard.h"

#define CHAR_DC1       0x11
#define CHAR_DC2       0x12
#define CHAR_ESC       0x1B
#define CHAR_ACK       0x06
#define CHAR_NAK       0x15
#define ABORT          0x04
#define BUTTON_MASK_T0 0x01
#define DATA_SIZE      256

typedef struct {
    uint8_t bytes[DATA_SIZE];
    uint8_t len;
} data_t;

typedef enum {
    Idle,
    Cmd,
    Data,
    Bcc,
    Resp,
} state_t;

static state_t state;
static uint8_t out;
static uint8_t cmd;
static uint8_t bcc;
static uint8_t len;
static uint8_t pos;
static data_t cmd_buf;
static data_t out_buf;

static uint8_t button_state;

/* ------------------------------------------------------------------
 * -- Function prototypes
 * ------------------------------------------------------------------
 */
void hal_mocked_spi_init(void);
uint8_t hal_mocked_spi_read_write(uint8_t send_byte);
void hal_mocked_sbuf_init(void);
uint8_t hal_mocked_sbuf_get_state(void);

static void data_clear(data_t *data);
static void data_append(data_t *data, uint8_t value);
static uint8_t data_at(data_t *data, uint8_t pos);
static uint8_t data_len(data_t *data);

static uint8_t exec_cmd(void);

static void idle(void);
static void next(state_t next_state);
static void resp(uint8_t next_out, state_t next_state);
static void error(void);

static uint8_t get_new_button_event(void);
static void print_ct_lcd(uint8_t *buf, uint8_t offset);

/* ------------------------------------------------------------------
 * -- Function implementations
 * ------------------------------------------------------------------
 */

/*
 * according to description in header file
 */
void hal_mocked_spi_init(void)
{
    idle();
}

/*
 * according to description in header file
 */
uint8_t hal_mocked_spi_read_write(uint8_t send_byte)
{
    uint8_t result = out;
    switch(state) {
        case Idle:
            if (send_byte == CHAR_DC1 || send_byte == CHAR_DC2) {
                cmd = send_byte;
                bcc = send_byte;
                next(Cmd);
            } else if (send_byte == 0x00) {
                idle();
            } else {
                error();
            }
            break;
        case Cmd:
            bcc += send_byte;
            len = send_byte;
            pos = 0;
            data_clear(&cmd_buf);
            next(Data);
            break;
        case Data:
            data_clear(&out_buf);
            if (len > pos) {
                bcc += send_byte;
                pos++;
                data_append(&cmd_buf, send_byte);
                next(Data);
            } else if (bcc == send_byte && exec_cmd()) {
                pos = 0;
                len = data_len(&out_buf);
                resp(CHAR_ACK, Resp);
            } else {
                error();
            }
            break;
        case Resp:
            if (send_byte == 0x00 && len > pos) {
            resp(data_at(&out_buf, pos++), Resp);
            } else if (send_byte == 0x00) {
                idle();
            } else {
                error();
            }
            break;
        case Bcc:
        default:
            error();
            break;
    }
    return result;
}

void hal_mocked_sbuf_init(void)
{
    button_state = 0x02;
}

uint8_t hal_mocked_sbuf_get_state(void)
{
    return get_new_button_event();
}

static void resp(uint8_t next_out, state_t next_state)
{
    out = next_out;
    state = next_state;
}

static void next(state_t next_state)
{
    out = 0x00;
    state = next_state;
}

static void idle(void)
{
    out = 0x00;
    state = Idle;
}

static void error(void)
{
    print_ct_lcd("Protocol error.", 20);
    out = CHAR_NAK;
    state = Idle;
}

static uint8_t exec_cmd(void)
{
    uint8_t ok_status = 0;
    uint8_t i;
    if (cmd == CHAR_DC1 || cmd == CHAR_DC2) {
        if (cmd == CHAR_DC1) {
            // "ESC Z L" print string
            if (cmd_buf.len > 7 && 0 == strncmp((char *)cmd_buf.bytes, "\x1bZL", 3)) {
                print_ct_lcd(cmd_buf.bytes+7, 0);
                print_ct_lcd("print_text", 20);
            }
            ok_status = 1;
        } else if (cmd == CHAR_DC2) {
            if (cmd_buf.len == 1 && cmd_buf.bytes[0] == 'S') {
                static uint8_t button_resp[] = { CHAR_DC1, 0x04, CHAR_ESC, 'A', 0x01, 0x01, 0x00 };
                strncpy((char *)out_buf.bytes, (char *)button_resp, sizeof(button_resp));
                out_buf.len = sizeof(button_resp);
                out_buf.bytes[5] = button_state;
                for(i = 0; i < sizeof(button_resp)-1; i++) {
                    out_buf.bytes[6] += out_buf.bytes[i];
                }
                ok_status = 1;
        } else {
            ok_status = 0;
            }
        }
    }
    return ok_status;
}

static void data_clear(data_t *data)
{
    data->len = 0;
}

static void data_append(data_t *data, uint8_t value)
{
        data->bytes[data->len++] = value;
}

static uint8_t data_at(data_t *data, uint8_t position)
{
    return data->bytes[position];
}

static uint8_t data_len(data_t *data)
{
    return data->len;
}

/*
 * Check if there has been a change in the state of
 * the T0 button. On each press of T0, we simulate 
 * the changing state of the touch button.
 */
static uint8_t get_new_button_event()
{
    static uint8_t button_T0_previous = 0;
    uint8_t event = 0;
    // 0 = no event, 1 = button down, 2 = button up
    if((!button_T0_previous && (CT_BUTTON & BUTTON_MASK_T0))) {
        button_T0_previous = 1;
        button_state = 3 - button_state;
        event = button_state;
    } else if(button_T0_previous && !(CT_BUTTON & BUTTON_MASK_T0)) {
        button_T0_previous = 0;
    }
    return event;
}

/*
 * Print string in buf to the CT-Board's LCD. Print max. n chars, 
 * starting from offset until the end of the LCD line.
 * Give offset = 20 to print on second line.
 */
static void print_ct_lcd(uint8_t *buf, uint8_t offset)
{
    uint8_t index = 0;
    uint8_t max_index;
    if(offset > 19){
        max_index = 39-offset;
    } else {
        max_index = 19-offset;
    }
    while(index <= max_index && buf[index] != 0){
        CT_LCD->ASCII[offset+index] = buf[index];
        index++;
    }
    return;
}

#endif // MOCKED_SPI_DISPLAY
