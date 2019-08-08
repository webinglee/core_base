lib_OBJ := $(OBJ)/$(PACKAGE_NAME)

lib-cfile := $(sort $(shell find $(SRC) -name *.c))
lib-ofile := $(lib-cfile:.c=.o)
lib-ofile := $(addprefix $(lib_OBJ)/,$(lib-ofile))
lib-hfile := $(shell find $(INC) -name *.h)
lib-dfile := $(lib-ofile:.o=.d)

-include $(lib-dfile)

$(PACKAGE_NAME)_LDFLAGS := $(LDPATH) -E

$(lib_OBJ)/%.o: %.c
	@echo + cc $<
	$(V)mkdir -p $(@D)
	$(V)$(CC) $(CCFLAGS) -c -o $@ $<