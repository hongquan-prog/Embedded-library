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


include $(MOD_COMPILE)

#######################################
# default goals
#######################################
$(MODULE_LIBS): $(ROOT_BUILD_DIR) $(BUILD_SUB_DIR)
	@for dir in $(MODULES); \
        do \
            $(call compile_module, $$dir, $(ROOT_BUILD_DIR)) \
        done

#######################################
# command goals
#######################################
$(MODULES): $(ROOT_BUILD_DIR) $(ROOT_BUILD_DIR)/$(MAKECMDGOALS)
	$(call compile_module, $@)

#######################################
# create directory
#######################################
$(ROOT_BUILD_DIR) $(BUILD_SUB_DIR):
	mkdir $@		

#######################################
# clean up
#######################################
clean:
	-rm -fR $(ROOT_BUILD_DIR)
  
#######################################
# dependencies
#######################################
-include $(wildcard $(ROOT_BUILD_DIR)/*.d)
