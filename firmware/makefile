# Name: makefile
# Project: rocket_ejection
# Author: Jonny Dyer
# Credits: Pascal Stang for avrlib

MCU = atmega328p
F_CPU = 16000000   # Hz

TRG = main

SRC = bmp280.c lis331.c lsm303d.c serialize.c $(TRG).c

# AVRLIB defines
AVRLIB_SRC = swTimer.c timer.c i2c.c buffer.c
AVRLIB = ../../../dev/avrlib/

# V-USB defines
VUSBLIB = usbdrv/
VUSBOBJ = usbdrv.o usbdrvasm.o oddebug.o

#put additional assembler source file here
ASRC =

#additional libraries and object files to link
LIB	=

#additional includes to compile
INC	=  

#assembler flags
ASFLAGS = -DF_CPU=$(F_CPU)

#compiler flags
CPFLAGS	= -g -Os -Wall -Wstrict-prototypes \
		  -I$(AVRLIB) -I$(VUSBLIB) -I. \
		  -DF_CPU=$(F_CPU) \
		  -Wa,-ahlms=$(<:.c=.lst)

LDFLAGS = -Wl,-Map=$(TRG).map,--cref# -lm

CC	= avr-gcc
AS	= avr-gcc -x assembler-with-cpp	
AVRDUDE = avrdude -c usbasp

SRC += $(addprefix $(AVRLIB), $(AVRLIB_SRC))
OBJ = $(ASRC:.s=.o) $(SRC:.c=.o) $(addprefix $(VUSBLIB),$(VUSBOBJ))
CPFLAGS += -mmcu=$(MCU)
ASFLAGS += -mmcu=$(MCU)
LDFLAGS += -mmcu=$(MCU)

###### RULES ########

all: $(TRG).elf $(TRG).hex $(TRG).ok

%.o : %.c 
	@echo "Building $<"
	$(CC) -c $(CPFLAGS) $< -o $@

%.S : %.c
	@echo "Building $<"
	$(CC) -S $(CPFLAGS) $< -o $@

%.o : %.S
	@echo "Assembling $<"
	$(AS) -c $(ASFLAGS) $< -o $@

%.elf : $(OBJ)
	@echo "Linking $@"
	$(CC) $(OBJ) $(LIB) $(LDFLAGS) -o $@

%.hex: %.elf
	avr-objcopy -j .text -j .data -O ihex -R .eeprom $< $@

%ok:
	avr-size $(TRG).elf
	@echo "Errors: none" 

help:
	@echo "make ........... to build $(TRG).hex"
	@echo "make fuse ...... to flash the fuses"
	@echo "make flash ..... to flash the firmware (use this on metaboard)"
	@echo "make clean ..... to delete objects and hex file"

fuse:
	@[ "$(FUSE_H)" != "" -a "$(FUSE_L)" != "" ] || \
		{ echo "*** Edit Makefile and choose values for FUSE_L and FUSE_H!"; exit 1; }
	$(AVRDUDE) -U hfuse:w:$(FUSE_H):m -U lfuse:w:$(FUSE_L):m

flash: $(TRG).hex
	$(AVRDUDE) -p$(MCU) -U flash:w:$(TRG).hex


clean::
	$(RM) $(OBJ)
	$(RM) $(SRC:.c=.s)
	$(RM) $(SRC:.c=.lst)
	$(RM) $(TRG).map
	$(RM) $(TRG).elf
	$(RM) $(TRG).cof
	$(RM) $(TRG).obj
	$(RM) $(TRG).a90
	$(RM) $(TRG).sym
	$(RM) $(TRG).eep
	$(RM) $(TRG).hex
	$(RM) $(TRG).bin
	@echo "Errors: none"
	
size:
	avr-size $(TRG).elf
		  
