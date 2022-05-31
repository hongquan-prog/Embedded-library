.PHONY: flash clean all

######################################
# module
######################################
MODULES := Driver \
Error \
DataStructure \
FileSystem \
MemoryManage \
OperationSystem

#######################################
# paths
#######################################
# Build path
ROOT_BUILD_DIR = build
BUILD_SUB_DIR := $(addprefix $(ROOT_BUILD_DIR)/, $(MODULES))

#######################################
# module
#######################################
MODULE_CFG := module-cfg.mk
MODULE_CMD := module-cmd.mk
MODULE_RULE := module-rule.mk
MODULE_COMPILE := module-compile.mk
MODULE_LIBS := $(addsuffix .a, $(MODULES))
MODULE_LIBS := $(addprefix $(DIR_BUILD)/lib, $(MODULE_LIBS))

# debug option
DEBUG := 1

#######################################
# Project
#######################################
TARGET := MetaOS
TARGET_DIR := Project/$(TARGET)
TARGET_BUILD_DIR := $(TARGET_DIR)/build
TARGET := $(TARGET_BUILD_DIR)/$(TARGET).bin


#######################################
# default goals
#######################################
include $(MODULE_COMPILE)
all: $(ROOT_BUILD_DIR) $(BUILD_SUB_DIR) Makefile
    # compile module
	@for dir in $(MODULES); \
    do \
        if [ -e "$$dir/Makefile" ]; then \
        	$(call compile_module, $$dir, $(ROOT_BUILD_DIR)) \
		elif [ -e "$$dir/makefile" ]; then \
		    $(call compile_module, $$dir, $(ROOT_BUILD_DIR)) \
		fi \
    done
    # compile peoject
	@cd $(TARGET_DIR) && $(MAKE) \
    DEBUG:=$(DEBUG)

#######################################
# download
#######################################
flash:
	@cd $(TARGET_DIR) && $(MAKE) $@

#######################################
# command goals
#######################################
$(MODULES): $(ROOT_BUILD_DIR) $(ROOT_BUILD_DIR)/$(MAKECMDGOALS)
	@$(call compile_module, $@, $(ROOT_BUILD_DIR))

#######################################
# create directory
#######################################
$(ROOT_BUILD_DIR) $(BUILD_SUB_DIR):
	mkdir $@		

#######################################
# clean up
#######################################
clean:
	-rm -fR $(ROOT_BUILD_DIR) $(TARGET_BUILD_DIR)

#######################################
# dependencies
#######################################
-include $(wildcard $(ROOT_BUILD_DIR)/*.d)
