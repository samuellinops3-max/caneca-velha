TITLE		:= CANECA BOMBA
APPID		:= CANE00001
CONTENTID	:= UP0001-CANE00001_00-CANECABOMBA00001

TARGET		:= $(CONTENTID)
BUILD		:= build
SOURCES		:= .
INCLUDES	:= .

CFILES		:= $(foreach dir,$(SOURCES),$(wildcard $(dir)/*.c))
OFILES		:= $(CFILES:.c=.o)

INCLUDE		:= $(foreach dir,$(INCLUDES),-I$(dir)) \
			-I$(PSL1GHT)/ppu/include \
			-I$(PSL1GHT)/portlibs/ppu/include

LIBDIRS		:= $(PSL1GHT)/ppu/lib $(PSL1GHT)/portlibs/ppu/lib
LIBPATHS	:= $(foreach dir,$(LIBDIRS),-L$(dir))
LIBS		:= -ltiny3d -lrsx -lgcm_sys -lio -lsysutil -lreality -lsysmodule -lm

include $(PSL1GHT)/ppu_rules

pkg: $(TARGET).pkg