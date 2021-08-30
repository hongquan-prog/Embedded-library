#include "ili9341.h"

static const uint8_t ili9341_reg1[] = {0x00, 0xC1, 0x30, 0xCF};
static const uint8_t ili9341_reg2[] = {0x64, 0x03, 0x12, 0x81, 0xED};
static const uint8_t ili9341_reg3[] = {0x85, 0x10, 0x7A, 0xE8};
static const uint8_t ili9341_reg4[] = {0x39, 0x2C, 0x00, 0x34, 0x02, 0xCB};
static const uint8_t ili9341_reg5[] = {0x20, 0xF7};
static const uint8_t ili9341_reg6[] = {0x00, 0x00, 0xEA};
static const uint8_t ili9341_reg7[] = {0x1B, 0xC0};
static const uint8_t ili9341_reg8[] = {0x01, 0xC1};
static const uint8_t ili9341_reg9[] = {0x30, 0x30, 0xC5};
static const uint8_t ili9341_reg10[] = {0xB7, 0xC7};
static const uint8_t ili9341_reg11[] = {0x48, 0x36};
static const uint8_t ili9341_reg12[] = {0x55, 0x3A};
static const uint8_t ili9341_reg13[] = {0x00, 0x1A, 0xB1};
static const uint8_t ili9341_reg14[] = {0x0A, 0xA2, 0xB6};
static const uint8_t ili9341_reg15[] = {0x00, 0xF2};
static const uint8_t ili9341_reg16[] = {0x01, 0x26};
static const uint8_t ili9341_reg17[] = {0x0F, 0x2A, 0x28, 0x08, 0x0E, 0x08, 0x54, 0xA9, 0x43, 0x0A, 0x0F, 0x00, 0x00, 0x00, 0x00, 0xE0};
static const uint8_t ili9341_reg18[] = {0x00, 0x15, 0x17, 0x07, 0x11, 0x06, 0x2B, 0x56, 0x3C, 0x05, 0x10, 0x0F, 0x3F, 0x3F, 0x0F, 0xE1};
static const uint8_t ili9341_reg19[] = {0x00, 0x00, 0x01, 0x3F, 0x2B};
static const uint8_t ili9341_reg20[] = {0x00, 0x00, 0x00, 0xEF, 0x2A};
static const uint8_t ili9341_reg21[] = {0x11};
static const uint8_t ili9341_madctl_portrait[] = {0x8, ILI9341_CMD_MADCTL};
static const uint8_t ili9341_caset_portrait[] = {0x00, 0x00, (ILI9341_240X320_WIDTH - 1) >> 8, (ILI9341_240X320_WIDTH - 1) & 0xFF, ILI9341_CMD_CASET};
static const uint8_t ili9341_raset_portrait[] = {0x00, 0x00, (ILI9341_240X320_HEIGHT - 1) >> 8, (ILI9341_240X320_HEIGHT - 1) & 0xFF, ILI9341_CMD_RASET};
static const uint8_t ili9341_madctl_landscape[] = {0x68, ILI9341_CMD_MADCTL};
static const uint8_t ili9341_caset_landscape[] = {0x00, 0x00, (ILI9341_320X240_WIDTH - 1) >> 8, (ILI9341_320X240_WIDTH - 1) & 0xFF, ILI9341_CMD_CASET};
static const uint8_t ili9341_raset_landscape[] = {0x00, 0x00, (ILI9341_320X240_HEIGHT - 1) >> 8, (ILI9341_320X240_HEIGHT - 1) & 0xFF, ILI9341_CMD_RASET};

void ili9341_write_cmd(int n, uint8_t* reg)
{
    uint16_t addr = reg[n];

    fmc_write_reg(addr);
    for (int i = 0; i < n; i++)
    {
        fmc_write_ram(reg[i]);
    }
}

static uint16_t ili9341_read_id()
{
    uint16_t ret = 0;

    fmc_write_reg(0xD3);
    ret = fmc_read_data();
    ret = fmc_read_data();
    ret = fmc_read_data();
    ret <<= 8;
    ret |= fmc_read_data();

    return ret;
}

static void ili9341_write_prepare(void)
{
    fmc_write_reg(ILI9341_CMD_RAMWR);
}

static void ili9341_set_cursor(uint16_t x, uint16_t y)
{
    fmc_write_reg(ILI9341_CMD_CASET);
    fmc_write_ram(x >> 8);
    fmc_write_ram(x & 0xff);
    fmc_write_reg(ILI9341_CMD_RASET);
    fmc_write_ram(y >> 8);
    fmc_write_ram(y & 0xff);
}

void ili9341_display_off(void)
{
    fmc_write_reg(ILI9341_CMD_DISPOFF);
}

void ili9341_display_on(void)
{
    fmc_write_reg(ILI9341_CMD_DISPON);
}

void ili9341_draw_pixel(int x, int y, uint16_t color)
{
    ili9341_set_cursor(x, y);
    ili9341_write_prepare();
    LCD->ram = color;
}

void ili9341_clear(uint32_t color)
{
    ili9341_set_cursor(0, 0);
    ili9341_write_prepare();
    for(int i = 0; i < ILI9341_240X320_HEIGHT; i++)
    {
        for(int j = 0; j < ILI9341_240X320_WIDTH; j++)
        {
            LCD->ram = color;
        }
    }
}

void ili9341_fill_rect(int x_begin, int y_begin, int x_end, int y_end, uint16_t* color)
{
    for(int i = y_begin; i <= y_end; i++)
    {
        ili9341_set_cursor(x_begin, i);
        ili9341_write_prepare();
        for(int j = x_begin; j <= x_end; j++)
        {
            LCD->ram = (*color);
            color++;
        }
    }
}

bool ili9341_init(uint32_t orientation)
{
    bool ret = false;
    
    ret = (ili9341_read_id() == ILI9341_ID);

    ili9341_write_cmd(3, (uint8_t *)ili9341_reg1);
    ili9341_write_cmd(4, (uint8_t *)ili9341_reg2);
    ili9341_write_cmd(3, (uint8_t *)ili9341_reg3);
    ili9341_write_cmd(5, (uint8_t *)ili9341_reg4);
    ili9341_write_cmd(1, (uint8_t *)ili9341_reg5);
    ili9341_write_cmd(2, (uint8_t *)ili9341_reg6);
    ili9341_write_cmd(1, (uint8_t *)ili9341_reg7);
    ili9341_write_cmd(1, (uint8_t *)ili9341_reg8);
    ili9341_write_cmd(2, (uint8_t *)ili9341_reg9);
    ili9341_write_cmd(1, (uint8_t *)ili9341_reg10);
    ili9341_write_cmd(1, (uint8_t *)ili9341_reg11);
    ili9341_write_cmd(1, (uint8_t *)ili9341_reg12);
    ili9341_write_cmd(2, (uint8_t *)ili9341_reg13);
    ili9341_write_cmd(2, (uint8_t *)ili9341_reg14);
    ili9341_write_cmd(1, (uint8_t *)ili9341_reg15);
    ili9341_write_cmd(1, (uint8_t *)ili9341_reg16);
    ili9341_write_cmd(15, (uint8_t *)ili9341_reg17);
    ili9341_write_cmd(15, (uint8_t *)ili9341_reg18);
    ili9341_write_cmd(4, (uint8_t *)ili9341_reg19);
    ili9341_write_cmd(4, (uint8_t *)ili9341_reg20);
    ili9341_write_cmd(0, (uint8_t *)ili9341_reg21);
    ili9341_delay(100);
    ili9341_display_on();
    /* Configure orientation */
    if (orientation == LCD_ORIENTATION_PORTRAIT)
    {
        ili9341_write_cmd(1, (uint8_t *)ili9341_madctl_portrait);
        ili9341_write_cmd(4, (uint8_t *)ili9341_caset_portrait);
        ili9341_write_cmd(4, (uint8_t *)ili9341_raset_portrait);
    }
    else
    {
        ili9341_write_cmd(1, (uint8_t *)ili9341_madctl_landscape);
        ili9341_write_cmd(4, (uint8_t *)ili9341_caset_landscape);
        ili9341_write_cmd(4, (uint8_t *)ili9341_raset_landscape);
    }
    ili9341_clear(LCD_WHITE);

    return ret;
}
