APP=main

TARGET=$(APP)
IMAGE=$(APP).hex
SRC=$(APP).c timer.c
OBJECTS=$(SRC:.c=.o)
DEPS=$(SRC:.c=.d)

ARCH_FLAGS=-mmcu=atmega328p
ARCH_FLAGS+=-DF_CPU=16000000UL
ARCH_FLAGS+=-Wall

CFLAGS=$(ARCH_FLAGS) -Os
LDFLAGS=$(ARCH_FLAGS)
LDFLAGS+=-Wl,-Map=map.map,--cref

CROSS=avr-
CC=$(CROSS)gcc
LD=$(CROSS)gcc
SIZE=$(CROSS)size
OBJCOPY=$(CROSS)objcopy
LIST=$(CROSS)objdump -xCedlSwz
GDB=$(CROSS)gdb

all: $(TARGET)

_:
	@echo $(CFLAGS)
	@echo $(LDFLAGS)

-include $(DEPS)

%.o: %.c
	@echo "CC $<"
	$(Q)$(CC) -MMD $(CFLAGS) -c -o $@ $<

$(TARGET): $(OBJECTS) Makefile
	@echo "LD $@"
	$(Q)$(LD) -o $@ $(OBJECTS) $(LDFLAGS)
	$(Q)$(OBJCOPY) -v -O ihex $@ $(IMAGE)
	$(Q)$(LIST) $@ > $(APP).lst
	$(Q)echo Executable SIZE:
	$(Q)$(SIZE) $@.hex
# We don't need the target, we have the hex image!
	$(Q)rm -fR $(APP)

.PHONY: clean download

download: $(TARGET)

clean:
	@echo "CLEAN"
	$(Q)rm -fR $(OBJECTS) $(TARGET) $(DEPS) $(APP).lst $(APP).bin $(APP).hex map.map
