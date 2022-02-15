.PHONY: all

MODULE := $(notdir $(realpath .))
CURRENT_BUILD_DIR := $(addsuffix /$(MODULE),$(BUILD_DIR))

SUB_MODULES := $(wildcard *)
SUB_MODULES := $(filter-out Makefile, $(SUB_MODULES))
SUB_MODULE_LIBS := $(addsuffix .a, $(SUB_MODULES))
SUB_MODULE_LIBS := $(addprefix $(CURRENT_BUILD_DIR)/lib, $(SUB_MODULE_LIBS))
BUILD_SUB_DIR := $(addprefix $(CURRENT_BUILD_DIR)/, $(SUB_MODULES))

include $(MOD_COMPILE)

all: $(CURRENT_BUILD_DIR) $(BUILD_SUB_DIR) Makefile
	@for dir in $(SUB_MODULES); \
    do \
        $(call compile_module, $$dir, $(CURRENT_BUILD_DIR)) \
    done

$(CURRENT_BUILD_DIR) $(BUILD_SUB_DIR):
	mkdir $@