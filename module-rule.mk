.PHONY: all

MODULE := $(notdir $(realpath .))
OUTPUT_DIR := $(addprefix $(BUILD_DIR)/,$(MODULE))
OUTPUT := $(addprefix $(BUILD_DIR)/lib,$(MODULE).a)

SRCS := $(wildcard $(SRC_DIR)/*$(TYPE_SRC))
SRCS := $(notdir $(SRCS))
OBJS := $(SRCS:$(TYPE_SRC)=$(TYPE_OBJ))
DEPS := $(SRCS:$(TYPE_SRC)=$(TYPE_DEP))
OBJS := $(addprefix $(OUTPUT_DIR)/,$(OBJS))
DEPS := $(addprefix $(OUTPUT_DIR)/,$(DEPS))

vpath %$(TYPE_SRC) $(SRC_DIR)
vpath %$(TYPE_INC) $(INC_DIR)

$(OUTPUT):$(OBJS)
	$(AR) $(ARFLAGS) $@ $^

$(OUTPUT_DIR):
	@$(MKDIR) $@

-include $(DEPS)
ifeq ("$(wildcard $(OUTPUT_DIR))","")
$(OUTPUT_DIR)/%$(TYPE_DEP):$(OUTPUT_DIR) %$(TYPE_SRC)
else
$(OUTPUT_DIR)/%$(TYPE_DEP):%$(TYPE_SRC)
endif
	$(CC) $(CFLAGS) -MM -E $(filter %$(TYPE_SRC), $^) | sed 's,\(.*\)\.o[ :]*,$(OUTPUT_DIR)/\1.o $@:,g' > $@

$(OUTPUT_DIR)/%$(TYPE_OBJ):%$(TYPE_SRC)
	$(CC) $(CFLAGS) -o $@ -c $(filter %$(TYPE_SRC),$^)

all: $(OUTPUT)