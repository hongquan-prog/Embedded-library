#include "sdram.h"

void sdram_write_cmd(uint8_t bank, uint8_t reg, uint16_t value, uint8_t refresh)
{
    FMC_SDRAM_CommandTypeDef cmd = {
    .CommandMode = reg,
    .CommandTarget = bank,
    .AutoRefreshNumber = refresh,
    .ModeRegisterDefinition = value
    };
    HAL_SDRAM_SendCommand(&hsdram1, &cmd, 0x1000);
}

void sdram_init(void)
{
    sdram_write_cmd(FMC_SDRAM_CMD_TARGET_BANK1, FMC_SDRAM_CMD_CLK_ENABLE, 0, 1);
    HAL_Delay(10);
    sdram_write_cmd(FMC_SDRAM_CMD_TARGET_BANK1, FMC_SDRAM_CMD_PALL, 0, 1);
    sdram_write_cmd(FMC_SDRAM_CMD_TARGET_BANK1, FMC_SDRAM_CMD_AUTOREFRESH_MODE, 0, 8);
    sdram_write_cmd(FMC_SDRAM_CMD_TARGET_BANK1, FMC_SDRAM_CMD_LOAD_MODE, SDRAM_MODEREG_BURST_LENGTH_1 | SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL | SDRAM_MODEREG_CAS_LATENCY_3 | SDRAM_MODEREG_OPERATING_MODE_STANDARD | SDRAM_MODEREG_WRITEBURST_MODE_SINGLE, 1);
    HAL_SDRAM_ProgramRefreshRate(&hsdram1, 823);
}


