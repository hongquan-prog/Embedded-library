#######################################
# module compile
#######################################
define compile_module
	cd $1 && $(MAKE) \
    DEBUG:=$(DEBUG) \
    BUILD_DIR:=$(abspath $2) \
    MODULE_CFG:=$(abspath $(MODULE_CFG)) \
    MODULE_RULE:=$(abspath $(MODULE_RULE)) \
    MODULE_CMD:=$(abspath $(MODULE_CMD)) \
    MODULE_COMPILE:=$(abspath $(MODULE_COMPILE)) \
    && cd .. ; 
endef