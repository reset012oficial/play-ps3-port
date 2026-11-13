#---------------------------------------------------------------------------------
# Makefile Base para Play! PS2 Emulator na PS3 (PSL1GHT) v2.3.1
#---------------------------------------------------------------------------------
.SUFFIXES:

ifeq ($(strip $(PSL1GHT)),)
PSL1GHT := /usr/local/ps3dev
endif

include $(PSL1GHT)/ppu_rules

TARGET		:= play_ps3
BUILD		:= build
SOURCES		:= . Source/ui_ps3
INCLUDES	:= . Source Source/ui_ps3

TITLE		:= Play! PS2 Emulator v2.3.1
APPID		:= PLAYPS300
CONTENTID	:= UP0001-$(APPID)_00-0000000000000001

CXXFLAGS	= -g -O2 -Wall -std=c++17 -fexceptions -D_GNU_SOURCE -D__PS3__ $(MACHDEP) $(INCLUDE)
LDFLAGS		= $(MACHDEP) -Wl,-Map,$(notdir $@).map

LIBS		:= -lio -lsysutil -lsysmodule -lstdc++ -lm

export OUTPUT	:= $(CURDIR)/$(TARGET)
export VPATH	:= $(foreach dir,$(SOURCES),$(CURDIR)/$(dir))
export DEPSDIR	:= $(CURDIR)/$(BUILD)

CFILES		:= $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
CPPFILES	:= $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))

export OFILES	:= $(CPPFILES:.cpp=.o) $(CFILES:.c=.o)
export INCLUDE	:= $(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) -I$(CURDIR)/$(BUILD) $(EXTRA_INCLUDES)

.PHONY: all pkg clean

all: $(BUILD) $(OUTPUT).self

$(BUILD):
	@[ -d $@ ] || mkdir -p $@

# Build sem recursão - compila direto
$(OUTPUT).self: $(OUTPUT).elf
	@$(PSL1GHT)/bin/ppu-strip -o $(OUTPUT).self $(OUTPUT).elf

$(OUTPUT).elf: $(addprefix $(BUILD)/,$(OFILES))
	@$(CXX) $(LDFLAGS) -o $@ $^ $(LIBS)

$(BUILD)/%.o: %.cpp
	@mkdir -p $(BUILD)
	@$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD)/%.o: %.c
	@mkdir -p $(BUILD)
	@$(CC) $(CFLAGS) -c $< -o $@

pkg: all
	@mkdir -p pkg/USRDIR
	@cp $(OUTPUT).self pkg/USRDIR/EBOOT.BIN
	@make_self_npdrm $(OUTPUT).elf pkg/USRDIR/EBOOT.BIN $(CONTENTID)
	@sfo.py --title "$(TITLE)" --appid "$(APPID)" -f $(PSL1GHT)/bin/sfo.xml pkg/PARAM.SFO
	@pkg.py --contentid $(CONTENTID) pkg/ $(TARGET).pkg

clean:
	@rm -fr $(BUILD) $(TARGET).elf $(TARGET).self $(TARGET).pkg pkg