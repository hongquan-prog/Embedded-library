MKDIR := mkdir

FLOAT-ABI := -mfloat-abi=hard
FPU = -mfpu=fpv5-d16
CPU = -mcpu=cortex-m7
MCU := $(CPU) -mthumb $(FPU) $(FLOAT-ABI)
OPT := -Og

AR := arm-none-eabi-ar 
ARFLAGS := crs

C_DEFS =  \
-DUSE_HAL_DRIVER \
-DSTM32F767xx

CC := arm-none-eabi-gcc
CFLAGS += $(addprefix -I, $(INC_DIR)) 
CFLAGS += $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
endif