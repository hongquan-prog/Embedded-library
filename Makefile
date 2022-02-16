.PHONY: flash clean all

######################################
# module
######################################
MODULES := Driver \
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
MOD_CFG := module-cfg.mk
MOD_CMD := module-cmd.mk
MOD_RULE := module-rule.mk
MOD_COMPILE := module-compile.mk
MODULE_LIBS := $(addsuffix .a, $(MODULES))
MODULE_LIBS := $(addprefix $(DIR_BUILD)/lib, $(MODULE_LIBS))

# debug option
DEBUG := 0

#######################################
# Project
#######################################
TARGET_DIR := Project
TARGET_BUILD_DIR := $(TARGET_DIR)/build
TARGET := $(TARGET_BUILD_DIR)/Project.bin


#######################################
# default goals
#######################################
include $(MOD_COMPILE)
all: $(ROOT_BUILD_DIR) $(BUILD_SUB_DIR) Makefile
    # compile module
	@for dir in $(MODULES); \
    do \
        $(call compile_module, $$dir, $(ROOT_BUILD_DIR)) \
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
