PREFIX = /usr/local
ARCH = h8300-elf
BINDER = $(PREFIX)/bin
ADDNAME = $(ARCH)-

AR	= $(BINDER)/$(ADDNAME)ar
AS	= $(BINDER)/$(ADDNAME)as
CC	= $(BINDER)/$(ADDNAME)gcc
LD	= $(BINDER)/$(ADDNAME)ld
NM	= $(BINDER)/$(ADDNAME)nm
OBJCOPY	= $(BINDER)/$(ADDNAME)objcopy
OBJDUMP	= $(BINDER)/$(ADDNAME)objdump
RANLIB = $(BINDER)/$(ADDNAME)ranlib
STRIP = $(BINDER)/$(ADDNAME)strip

H8WRITE = ../../tools/h8write/h8write
H8WRITE_SERDEV = /dev/serial/by-id/usb-FTDI_USB_HS_SERIAL_CONVERTER_FT1S8XCY-if00-port0

OBJS = vector.o startup.o main.o
OBJS += lib.o serial.o xmodem.o

TARGET = kzload

CFLAGS = -Wall -mh -nostdinc -nostdlib -fno-builtin
#CFLAGS += -mint32
CFLAGS += -I.
#CFLAGS += -g
CFLAGS += -Os
CFLAGS += -DKZLOAD

LFLAGS = -static -T ld.scr -L.

.SUFFIXES: .c .o
.SUFFIXES: .s .o

all :	$(TARGET)

$(TARGET) :	$(OBJS)
		$(CC) $(OBJS) -o $(TARGET) $(CFLAGS) $(LFLAGS)
		cp $(TARGET) $(TARGET).elf
		$(STRIP) $(TARGET)

.c.o : 		$<
		$(CC) -c $(CFLAGS) $<

.s.o :		$<
		$(CC) -c $(CFLAGS) $<

$(TARGET).mot : $(TARGET)
		$(OBJCOPY) -O srec $(TARGET) $(TARGET).mot

image : 	$(TARGET).mot

write : 	$(TARGET).mot
		$(H8WRITE) -3069 -f20 $(TARGET).mot $(H8WRITE_SERDEV)

clean : 
		rm -f $(OBJS) $(TARGET) $(TARGET).elf $(TARGET).mot
