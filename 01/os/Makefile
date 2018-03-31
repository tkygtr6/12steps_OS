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


OBJS = startup.o main.o
OBJS += lib.o serial.o interrupt.o

# sources of kozos
OBJS += kozos.o syscall.o test08_1.o

TARGET = kozos

CFLAGS = -Wall -mh -nostdinc -nostdlib -fno-builtin
#CFLAGS += -mint32
CFLAGS += -I.
#CFLAGS += -g
CFLAGS += -Os
CFLAGS += -DKOZOS

LFLAGS = -static -T ld.scr -L.

.SUFFIXES: .c .o
.SUFFIXES: .s .o
.SUFFIXES: .S .o

all :	$(TARGET)

$(TARGET) :	$(OBJS)
		$(CC) $(OBJS) -o $(TARGET) $(CFLAGS) $(LFLAGS)
		cp $(TARGET) $(TARGET).elf
		$(STRIP) $(TARGET)

.c.o : 		$<
		$(CC) -c $(CFLAGS) $<

.s.o :		$<
		$(CC) -c $(CFLAGS) $<

.S.o :		$<
		$(CC) -c $(CFLAGS) $<

clean : 
		rm -f $(OBJS) $(TARGET) $(TARGET).elf
