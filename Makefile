#MK_CONF := $(CONF)/

# Verbose handler
ifndef VERBOSE
	V := @ # "V = " for verbose, "V = @" for non-verbose
else
	V :=
endif

# Debug handler
DEBUG := 1
ifdef DEBUG
	STABS := -D_DEBUG -D_FORWARD -D_WRAP
else
	STABS := -DNDEBUG
endif

# Debug info handler
DEBUG_INFO := 1
ifdef DEBUG_INFO
	DEBUG_INFO_FLAGS := -g
else
	DEBUG_INFO_FLAGS :=
endif

# Optimize handler
ifndef O_LEV
	O_LEV := 0
endif

CCFLAGS :=
GCCPATH := /usr/lib/ccache
GCCPREFIX :=

# tool chain def
MAKE	:= make
CC	:= $(GCCPATH)/$(GCCPREFIX)gcc
CXX	:= $(GCCPATH)/$(GCCPREFIX)g++
AS	:= $(GCCPATH)/$(GCCPREFIX)as
AR	:= $(GCCPATH)/$(GCCPREFIX)ar
LD	:= $(GCCPATH)/$(GCCPREFIX)ld
OBJCOPY := $(GCCPATH)/$(GCCPREFIX)objcopy
OBJDUMP := $(GCCPATH)/$(GCCPREFIX)objdump
NM	:= $(GCCPATH)/$(GCCPREFIX)nm
CPP	:= $(GCCPATH)/$(GCCPREFIX)cpp

SRC := src
INC := include
OBJ := obj

PACKAGE_NAME := core_base
LIBNAME := lib$(PACKAGE_NAME).so

OPT_LDPATH :=
OPT_CCFLAGS :=
OPT_LDFLAGS :=

DEP_CCFLAGS :=
DEP_LDFLAGS :=

LIB_LDFLAGS := -shared
LDPATH := -L$(OBJ) $(OPT_LDPATH)
PRE_LDFLAGS := $(LDPATH) $(DEP_LDFLAGS) $(OPT_LDFLAGS) -pthread
LDFLAGS := $(LIB_LDFLAGS) $(PRE_LDFLAGS)

 #-Wpointer-arith //Make all pointer-arith into errors.
 #-Werror //Make all warnings into errors.
CCFLAGS := $(CCFLAGS) -O$(O_LEV) -fstack-protector -Wno-int-to-pointer-cast  \
	-Wno-error=deprecated-declarations -Wextra -fdiagnostics-color=always \
	-I$(INC) -MD -Wall -Wno-unused -fPIC $(STABS) $(DEBUG_INFO_FLAGS) $(DEP_CCFLAGS) $(OPT_CCFLAGS)

-include $(SRC)/Makefile

PACKAGE_NAME-framework := $(lib-ofile)

$(LIBNAME): $(PACKAGE_NAME-framework)
	@echo + generate $(PACKAGE_NAME) from $^
	$(V)$(CC) -Wl,--as-needed -o $@ $^ $(LDFLAGS)
	@echo "$(PACKAGE_NAME) build successfully!"

install:
	@echo "$(PACKAGE_NAME) install has not implementation!"

uninstall:
	@echo "$(PACKAGE_NAME) uninstall has not implementation!"

clean:
	-rm -f $(LIBNAME)
	-rm -fr $(OBJ)