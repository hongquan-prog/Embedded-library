#include "nt35510.h"

static const uint8_t nt35510_reg[] = {0x55, 0xAA, 0x52, 0x08, 0x01, 0xF0};
static const uint8_t nt35510_reg1[] = {0x0D, 0x0D, 0x0D, 0xB0};
static const uint8_t nt35510_reg2[] = {0x34, 0x34, 0x34, 0xB6};
static const uint8_t nt35510_reg3[] = {0x0D, 0x0D, 0x0D, 0xB1};
static const uint8_t nt35510_reg4[] = {0x34, 0x34, 0x34, 0xB7};
static const uint8_t nt35510_reg5[] = {0x00, 0x00, 0x00, 0xB2};
static const uint8_t nt35510_reg6[] = {0x24, 0x24, 0x24, 0xB8};
static const uint8_t nt35510_reg7[] = {0x01, 0xBF};
static const uint8_t nt35510_reg8[] = {0x0F, 0x0F, 0x0F, 0xB3};
static const uint8_t nt35510_reg9[] = {0x34, 0x34, 0x34, 0xB9};
static const uint8_t nt35510_reg10[] = {0x08, 0x08, 0x08, 0xB5};
static const uint8_t nt35510_reg12[] = {0x24, 0x24, 0x24, 0xBA};
static const uint8_t nt35510_reg13[] = {0x00, 0x78, 0x00, 0xBC};
static const uint8_t nt35510_reg14[] = {0x00, 0x78, 0x00, 0xBD};
static const uint8_t nt35510_reg15[] = {0x00, 0x64, 0xBE};
static const uint8_t nt35510_reg16[] = {0x55, 0xAA, 0x52, 0x08, 0x00, 0xF0};
static const uint8_t nt35510_reg17[] = {0xCC, 0x00, 0xB1};
static const uint8_t nt35510_reg18[] = {0x05, 0xB6};
static const uint8_t nt35510_reg19[] = {0x51, 0xB5};
static const uint8_t nt35510_reg20[] = {0x70, 0x70, 0xB7};
static const uint8_t nt35510_reg21[] = {0x01, 0x03, 0x03, 0x03, 0xB8};
static const uint8_t nt35510_reg22[] = {0x00, 0x00, 0x00, 0xBC};
static const uint8_t nt35510_reg23[] = {0x03, 0x00, 0x00, 0xCC};
static const uint8_t nt35510_reg24[] = {0x01, 0xBA};
static const uint8_t nt35510_madctl_portrait[] = {0x00, NT35510_CMD_MADCTL};
static const uint8_t nt35510_caset_portrait[] = {0x00, 0x00, (NT35510_480X800_WIDTH - 1) >> 8, (NT35510_480X800_WIDTH - 1) & 0xFF, NT35510_CMD_CASET};
static const uint8_t nt35510_raset_portrait[] = {0x00, 0x00, (NT35510_480X800_HEIGHT - 1) >> 8, (NT35510_480X800_HEIGHT - 1) & 0xFF, NT35510_CMD_RASET};
static const uint8_t nt35510_madctl_landscape[] = {0x60, NT35510_CMD_MADCTL};
static const uint8_t nt35510_caset_landscape[] = {0x00, 0x00, (NT35510_800X480_WIDTH - 1) >> 8, (NT35510_800X480_WIDTH - 1) & 0xFF, NT35510_CMD_CASET};
static const uint8_t nt35510_raset_landscape[] = {0x00, 0x00, (NT35510_800X480_HEIGHT - 1) >> 8, (NT35510_800X480_HEIGHT - 1) & 0xFF, NT35510_CMD_RASET};
static const uint8_t nt35510_reg26[] = {0x00, NT35510_CMD_TEEON};  /* Tear on */
static const uint8_t nt35510_reg27[] = {0x00, NT35510_CMD_SLPOUT}; /* Sleep out */
static const uint8_t nt35510_reg30[] = {0x00, NT35510_CMD_DISPON};
static const uint8_t nt35510_reg31[] = {0x7F, NT35510_CMD_WRDISBV};
static const uint8_t nt35510_reg32[] = {0x2C, NT35510_CMD_WRCTRLD};
static const uint8_t nt35510_reg33[] = {0x02, NT35510_CMD_WRCABC};
static const uint8_t nt35510_reg34[] = {0xFF, NT35510_CMD_WRCABCMB};
static const uint8_t nt35510_reg35[] = {0x00, NT35510_CMD_RAMWR};
static const uint8_t nt35510_reg36[] = {NT35510_COLMOD_RGB565, NT35510_CMD_COLMOD};
static const uint8_t nt35510_reg37[] = {NT35510_COLMOD_RGB888, NT35510_CMD_COLMOD};

static uint16_t nt35510_read_id(void)
{
    uint16_t ret;
    fmc_write_reg(0XDA00);
    ret = fmc_read_data();
    fmc_write_reg(0XDB00);
    ret = fmc_read_data();
    ret <<= 8;
    fmc_write_reg(0XDC00);
    ret |= fmc_read_data();
    return ret;
}

void nt35510_write_cmd(int n, uint8_t* reg)
{
    uint16_t addr = reg[n] << 8;

    for (int i = 0; i < n; i++)
    {
        fmc_write_reg_and_ram(addr + i, reg[i]);
    }
}

void nt35510_display_off(void)
{
    fmc_write_reg(NT35510_CMD_DISPOFF << 8);
}
void nt35510_display_on(void)
{
    fmc_write_reg(NT35510_CMD_DISPON << 8);
}

static void nt35510_set_cursor(uint16_t x, uint16_t y)
{
    fmc_write_reg_and_ram(NT35510_CMD_RASET << 8, y >> 8);
    fmc_write_reg_and_ram((NT35510_CMD_RASET << 8) + 1, y & 0xff);
    fmc_write_reg_and_ram(NT35510_CMD_CASET << 8, x >> 8);
    fmc_write_reg_and_ram((NT35510_CMD_CASET << 8) + 1, x & 0xff);
}

static void nt35510_write_prepare(void)
{
    fmc_write_reg(NT35510_CMD_RAMWR << 8);
}

void nt35510_draw_pixel(int x, int y, uint16_t color)
{
    nt35510_set_cursor(x, y);
    nt35510_write_prepare();
    LCD->ram = color;
}

void nt35510_clear(uint32_t color)
{
    nt35510_set_cursor(0, 0);
    nt35510_write_prepare();
    for(int i = 0; i < NT35510_800X480_WIDTH; i++)
    {
        for(int j = 0; j < NT35510_800X480_HEIGHT; j++)
        {
            LCD->ram = color;
        }
    }
}

void nt35510_fill_rect(int x_begin, int y_begin, int x_end, int y_end, uint16_t* color)
{
    for(int i = y_begin; i <= y_end; i++)
    {
        nt35510_set_cursor(x_begin, i);
        nt35510_write_prepare();
        for(int j = x_begin; j <= x_end; j++)
        {
            LCD->ram = (*color);
            color++;
        }
    }
}

bool nt35510_init(uint32_t ColorCoding, uint32_t orientation)
{
    bool ret = false;

    ret = (nt35510_read_id() == NT35510_ID);

    nt35510_write_cmd(5, (uint8_t *)nt35510_reg);  /* LV2:  Page 1 enable */
    nt35510_write_cmd(3, (uint8_t *)nt35510_reg1); /* AVDD: 5.2V */
    nt35510_write_cmd(3, (uint8_t *)nt35510_reg2); /* AVDD: Ratio */
    nt35510_write_cmd(3, (uint8_t *)nt35510_reg3); /* AVEE: -5.2V */
    nt35510_write_cmd(3, (uint8_t *)nt35510_reg4); /* AVEE: Ratio */
    nt35510_write_cmd(3, (uint8_t *)nt35510_reg5); /* VCL: -2.5V */
    nt35510_write_cmd(3, (uint8_t *)nt35510_reg6); /* VCL: Ratio */
    nt35510_write_cmd(1, (uint8_t *)nt35510_reg7); /* VGH: 15V (Free Pump) */
    nt35510_write_cmd(3, (uint8_t *)nt35510_reg8);
    nt35510_write_cmd(3, (uint8_t *)nt35510_reg9);  /* VGH: Ratio */
    nt35510_write_cmd(3, (uint8_t *)nt35510_reg10); /* VGL_REG: -10V */
    nt35510_write_cmd(3, (uint8_t *)nt35510_reg12); /* VGLX: Ratio */
    nt35510_write_cmd(3, (uint8_t *)nt35510_reg13); /* VGMP/VGSP: 4.5V/0V */
    nt35510_write_cmd(3, (uint8_t *)nt35510_reg14); /* VGMN/VGSN:-4.5V/0V */
    nt35510_write_cmd(2, (uint8_t *)nt35510_reg15); /* VCOM: -1.325V */

    /* ************************************************************************** */
    /* Proprietary DCS Initialization                                             */
    /* ************************************************************************** */
    nt35510_write_cmd(5, (uint8_t *)nt35510_reg16); /* LV2: Page 0 enable */
    nt35510_write_cmd(2, (uint8_t *)nt35510_reg17); /* Display control */
    nt35510_write_cmd(1, (uint8_t *)nt35510_reg18); /* Src hold time */
    nt35510_write_cmd(1, (uint8_t *)nt35510_reg19);
    nt35510_write_cmd(2, (uint8_t *)nt35510_reg20); /* Gate EQ control */
    nt35510_write_cmd(4, (uint8_t *)nt35510_reg21); /* Src EQ control(Mode2) */
    nt35510_write_cmd(3, (uint8_t *)nt35510_reg22); /* Inv. mode(2-dot) */
    nt35510_write_cmd(3, (uint8_t *)nt35510_reg23);
    nt35510_write_cmd(1, (uint8_t *)nt35510_reg24);
    /* Tear on */
    nt35510_write_cmd(1, (uint8_t *)nt35510_reg26);

    /* ************************************************************************** */
    /* Standard DCS Initialization                                                */
    /* ************************************************************************** */

    /* Add a delay, otherwise MADCTL not taken */
    nt35510_delay(120);

    /* Configure orientation */
    if (orientation == LCD_ORIENTATION_PORTRAIT)
    {
        nt35510_write_cmd(1, (uint8_t *)nt35510_madctl_portrait);
        nt35510_write_cmd(4, (uint8_t *)nt35510_caset_portrait);
        nt35510_write_cmd(4, (uint8_t *)nt35510_raset_portrait);
    }
    else
    {
        nt35510_write_cmd(1, (uint8_t *)nt35510_madctl_landscape);
        nt35510_write_cmd(4, (uint8_t *)nt35510_caset_landscape);
        nt35510_write_cmd(4, (uint8_t *)nt35510_raset_landscape);
    }

    nt35510_write_cmd(1, (uint8_t *)nt35510_reg27);
    /* Wait for sleep out exit */
    nt35510_delay(120);
    switch (ColorCoding)
    {
    case NT35510_FORMAT_RBG565:
        /* Set Pixel color format to RGB565 */
        nt35510_write_cmd(1, (uint8_t *)nt35510_reg36);
        fmc_write_reg_and_ram(NT35510_CMD_COLMOD << 8, NT35510_COLMOD_RGB565);
        break;
    case NT35510_FORMAT_RGB888:
        /* Set Pixel color format to RGB888 */
        nt35510_write_cmd(1, (uint8_t *)nt35510_reg37);
        fmc_write_reg_and_ram(NT35510_CMD_COLMOD << 8, NT35510_COLMOD_RGB565);
        break;
    default:
        /* Set Pixel color format to RGB888 */
        nt35510_write_cmd(1, (uint8_t *)nt35510_reg36);
        fmc_write_reg_and_ram(NT35510_CMD_COLMOD << 8, NT35510_COLMOD_RGB565);
        break;
    }
    

    /** CABC : Content Adaptive Backlight Control section start >> */
    /* Note : defaut is 0 (lowest Brightness), 0xFF is highest Brightness, try 0x7F : intermediate value */
    nt35510_write_cmd(1, (uint8_t *)nt35510_reg31);
    /* defaut is 0, try 0x2C - Brightness Control Block, Display Dimming & BackLight on */
    nt35510_write_cmd(1, (uint8_t *)nt35510_reg32);
    /* defaut is 0, try 0x02 - image Content based Adaptive Brightness [Still Picture] */
    nt35510_write_cmd(1, (uint8_t *)nt35510_reg33);
    /* defaut is 0 (lowest Brightness), 0xFF is highest Brightness */
    nt35510_write_cmd(1, (uint8_t *)nt35510_reg34);
    /* Display on */
    nt35510_write_cmd(1, (uint8_t *)nt35510_reg30);
    /* Send Command GRAM memory write (no parameters) : this initiates frame write via other DSI commands sent by */
    /* DSI host from LTDC incoming pixels in video mode */
    nt35510_write_cmd(1, (uint8_t *)nt35510_reg35);

    nt35510_clear(LCD_WHITE);

    return ret;
}

uint8_t nt35510_deinit(void)
{
  static const uint8_t nt35510_reg30b[] = {0x00, NT35510_CMD_DISPOFF};
  static const uint8_t nt35510_reg27b[] = {0x00, NT35510_CMD_SLPIN};
  /* Display off */
  nt35510_write_cmd(0, (uint8_t *)nt35510_reg30b);
  nt35510_delay(120);
  /* Sleep in */
  nt35510_write_cmd(0, (uint8_t *)nt35510_reg27b);
  return 0;
}