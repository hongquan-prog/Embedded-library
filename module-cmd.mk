MKDIR := mkdir

FLOAT-ABI := -mfloat-abi=hard
FPU := -mfpu=fpv4-sp-d16
CPU := -mcpu=cortex-m4
MCU := $(CPU) -mthumb $(FPU) $(FLOAT-ABI)
OPT := -Og

AR := arm-none-eabi-ar 
ARFLAGS := crs

CC := arm-none-eabi-gcc
CFLAGS += $(addprefix -I, $(INC_DIR))
CFLAGS += $(MCU) $(OPT) -Wall -fdata-sections -ffunction-sections

ifeq ($(DEBUG), true)
CFLAGS += -g -gdwarf-2
endif