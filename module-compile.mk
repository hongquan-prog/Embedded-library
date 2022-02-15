#######################################
# module compile
#######################################
define compile_module
	cd $1 && $(MAKE) \
    DEBUG:=$(DEBUG) \
    BUILD_DIR:=$(abspath $2) \
    MOD_CFG:=$(abspath $(MOD_CFG)) \
    MOD_RULE:=$(abspath $(MOD_RULE)) \
    MOD_CMD:=$(abspath $(MOD_CMD)) \
	MOD_COMPILE:=$(abspath $(MOD_COMPILE))\
    && cd .. ;
endef