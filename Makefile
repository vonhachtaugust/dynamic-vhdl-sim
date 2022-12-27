PROJECT_ROOT = $(dir $(abspath $(lastword $(MAKEFILE_LIST))))

# Edit these to your package manager 
HOMEBREW_ARM = /Users/developer/Software/homebrew
HOMEBREW_x86 = /Users/developer/Software/x86_64/homebrew

# 3PPs
LWS_INCLUDE = $(HOMEBREW_ARM)/Cellar/libwebsockets/4.3.2/include
LWS_LIB = $(HOMEBREW_ARM)/Cellar/libwebsockets/4.3.2/lib

OPENSSL_INCLUDE = $(HOMEBREW_ARM)/Cellar/openssl@3/3.0.7/include
OPENSSL_LIB = $(HOMEBREW_ARM)/Cellar/openssl@3/3.0.7/lib

TARGET = vpi-server.vpi
TOP = tb
BUILD = build

VHD += vhd/ent.vhd
VHD += vhd/tb.vhd

SRC += src/queue.c
SRC += src/server.c
SRC += src/vpi.c

OBJS = $(addprefix $(BUILD)/, $(notdir $(SRC:.c=.o)))

# Using pkg-config to auto insert include and library 
CFLAGS += -W -Wall -O2 -Isrc -I$(OPENSSL_INCLUDE) -I$(LWS_INCLUDE)
LDFLAGS = -L$(LWS_LIB) -lwebsockets -lpthread

GHDL = ghdl
COMPILER = gcc
LINKER = gcc

GHDL_COMPILE = $(GHDL) --vpi-compile $(COMPILER)
GHDL_LINK = $(GHDL) --vpi-link $(LINKER)

all: $(BUILD)/$(TARGET)

$(BUILD)/%.o: src/%.c
	@mkdir -p $(@D)
	$(GHDL_COMPILE) $(CFLAGS) -c -o $@ $<

$(BUILD)/$(TARGET): $(OBJS)
	@echo Building target $(TARGET) in directory $(BUILD) using objects $(OBJS)
	$(GHDL_LINK) $(CFLAGS) $^ -o $@ $(LDFLAGS)

exec: $(BUILD)/$(TARGET)
	$(GHDL) -a --std=08 $(VHD)
	$(GHDL) -e --std=08 -o $(TOP) $(TOP)
	$(GHDL) -r --std=08 $(TOP) --vpi=./$(BUILD)/$(TARGET) --vpi-trace=trace.txt --wave=wave.ghw

clean:
	@rm -rfv $(BUILD)
	@rm -vf work-*.cf
	@rm -vf *.o
	@rm -vf test
	@rm -vf *.vcd
	@rm -vf *.ghw
	@rm -vf trace.txt
	