TARGET		:= CupheadVelho
TITLE		:= Cuphead Meio Velho PS3
APPID		:= CUPH00001
CONTENT_ID	:= UP0001-$(APPID)_00-0000000000000000

ifeq ($(strip $(PS3DEV)),)
  $(error "Por favor, defina a variavel de ambiente PS3DEV")
endif

include $(PS3DEV)/make/ps3.mk
include $(PS3DEV)/make/ppu.mk

SOURCE		:= .
OBJS		:= main.o

# Adicionado as bibliotecas de vídeo e controle necessárias do PS3
LIBS		+= -lrsx -lsysutil -lio
CFLAGS		+= -O2 -Wall
INCLUDES	+= 

.PHONY: all clean

all: $(TARGET).pkg

$(TARGET).elf: $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) $(LIBS) -o $@

$(TARGET).self: $(TARGET).elf
	@$(SFO) -f sfo.xml fake_npdata.sfo
	@$(PPU-STRIP) -s $<
	@$(MAKE_SELF) $< $@

$(TARGET).pkg: $(TARGET).self
	@mkdir -p pkg/USRDIR
	@cp $(TARGET).self pkg/USRDIR/EBOOT.BIN
	@$(SFO) -f sfo.xml pkg/PARAM.SFO
	@$(MAKE_PKG) --content-id=$(CONTENT_ID) pkg/ $@
	@rm -rf pkg

clean:
	@rm -rf $(OBJS) $(TARGET).elf $(TARGET).self $(TARGET).pkg fake_npdata.sfo
