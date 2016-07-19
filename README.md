## Setup instructions

### Program fuses
```
avrdude -c programmer_id -p atmega328p -P port_id -b 19200 -U lfuse:w:0xCF:m  hfuse:w:0xD9:m  efuse:w:0xFF:m  lock:w:0xFF:m
```

### Program firmware
```
make all
avrdude -c programmer_id -p atmega328p -P port_id -b 19200 -U flash:w:main.hex
```
