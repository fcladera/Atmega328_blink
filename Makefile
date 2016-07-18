APP=main

TARGET=$(APP).hex
#SRC=$(foreach m, $(MODULES), $(wildcard $(m)/src/*.c)) $(wildcard $(APP)/*/src/*.c)
SRC=$(APP).c timer.c
#INCLUDES=$(foreach m, $(MODULES), -I$(m)/inc) -Ilpc_chip_43xx/inc/usbd/ $(foreach i, $(wildcard $(APP)/*/inc), -I$(i))
#_DEFINES=$(foreach m, $(DEFINES), -D$(m))
OBJECTS=$(SRC:.c=.o)
DEPS=$(SRC:.c=.d)
#LDSCRIPT=ldscript/ciaa_lpc4337.ld

ARCH_FLAGS=-mmcu=atmega328p
ARCH_FLAGS+=-DF_CPU=16000000UL
ARCH_FLAGS+=-Wall

CFLAGS=$(ARCH_FLAGS) -Os
LDFLAGS=$(ARCH_FLAGS)
LDFLAGS+=-Wl,-Map=map.map,--cref
#LDFLAGS+=$(foreach l, $(LIBS), -l$(l))

all: $(TARGET)

_:
	@echo $(CFLAGS)
	@echo $(LDFLAGS)

CROSS=avr-
CC=$(CROSS)gcc
LD=$(CROSS)gcc
SIZE=$(CROSS)size
OBJCOPY=$(CROSS)objcopy
LIST=$(CROSS)objdump -xCedlSwz
GDB=$(CROSS)gdb

-include $(DEPS)

%.o: %.c
	@echo "CC $<"
	$(Q)$(CC) -MMD $(CFLAGS) -c -o $@ $<

$(TARGET): $(OBJECTS) Makefile
	@echo "LD $@"
	$(Q)$(LD) -o $@ $(OBJECTS) $(LDFLAGS)
	$(Q)$(OBJCOPY) -v -O binary $@ $(APP).bin
	$(Q)$(LIST) $@ > $(APP).lst
	$(Q)echo Executable SIZE:
	$(Q)$(SIZE) $@

.PHONY: clean download erase

download: $(TARGET)

erase:

clean:
	@echo "CLEAN"
	$(Q)rm -fR $(OBJECTS) $(TARGET) $(DEPS) $(APP).lst $(APP).bin map.map
