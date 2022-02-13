.PHONY:$(MODULES)

######################################
# module
######################################
MODULES := DataStructure

#######################################
# paths
#######################################
# Build path
BUILD_DIR = build
BUILD_SUB_DIR := $(addprefix $(BUILD_DIR)/, $(MODULES))

#######################################
# module
#######################################
MOD_CFG := module-cfg.mk
MOD_CMD := module-cmd.mk
MOD_RULE := module-rule.mk
MODULE_LIB := $(addsuffix .a, $(MODULES))
MODULE_LIB := $(addprefix $(DIR_BUILD)/lib, $(MODULE_LIB))

#######################################
# module compile
#######################################
define compile_module
	cd $1 && $(MAKE) \
        DEBUG:=$(DEBUG) \
        BUILD_DIR:=$(abspath $(BUILD_DIR)) \
        MOD_CFG:=$(abspath $(MOD_CFG)) \
        MOD_RULE:=$(abspath $(MOD_RULE)) \
        MOD_CMD:=$(abspath $(MOD_CMD)) \
        && cd .. ;
endef

#######################################
# default goals
#######################################
$(MODULE_LIB): $(BUILD_DIR) $(BUILD_SUB_DIR)
	@for dir in $(MODULES); \
        do \
            $(call compile_module, $$dir) \
        done

#######################################
# command goals
#######################################
$(MODULES): $(BUILD_DIR) $(BUILD_DIR)/$(MAKECMDGOALS)
	$(call compile_module, $@)

#######################################
# create directory
#######################################
$(BUILD_DIR) $(BUILD_SUB_DIR):
	mkdir $@		

#######################################
# clean up
#######################################
clean:
	-rm -fR $(BUILD_DIR)
  
#######################################
# dependencies
#######################################
-include $(wildcard $(BUILD_DIR)/*.d)

