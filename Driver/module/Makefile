.PHONY: all

MODULE := $(notdir $(realpath .))
CURRENT_BUILD_DIR := $(addsuffix /$(MODULE),$(BUILD_DIR))

SUB_MODULES := $(wildcard *)
SUB_MODULES := $(filter-out Makefile, $(SUB_MODULES))
SUB_MODULE_LIBS := $(addsuffix .a, $(SUB_MODULES))
SUB_MODULE_LIBS := $(addprefix $(CURRENT_BUILD_DIR)/lib, $(SUB_MODULE_LIBS))
BUILD_SUB_DIR := $(addprefix $(CURRENT_BUILD_DIR)/, $(SUB_MODULES))

INTERFACE_INC := $(wildcard ../interface/*)
INTERFACE_INC := $(filter-out ../interface/Makefile, $(INTERFACE_INC))
INTERFACE_INC := $(realpath $(INTERFACE_INC))

export INTERFACE_INC
include $(MODULE_COMPILE)

all: $(CURRENT_BUILD_DIR) $(BUILD_SUB_DIR) Makefile
	@for dir in $(SUB_MODULES); \
    do \
        if [ -e "$$dir/Makefile" ]; then \
        	$(call compile_module, $$dir, $(CURRENT_BUILD_DIR)) \
		elif [ -e "$$dir/makefile" ]; then \
		    $(call compile_module, $$dir, $(CURRENT_BUILD_DIR)) \
		fi \
    done

$(CURRENT_BUILD_DIR) $(BUILD_SUB_DIR):
	mkdir $@