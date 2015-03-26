# Set up you environment here

TARGET_ROOT = @target_root@
CROSS = arm-linux-

CC =     $(CROSS)gcc
AR =     $(CROSS)ar
LD =     $(CROSS)ld
NM =     $(CROSS)nm
RANLIB = $(CROSS)ranlib

JDKPATH = /usr/java/j2sdk1.4.2_09/include/linux

# Modify here 
OBJ = MWGateway.o

all: sdk

sdk: $(OBJ)
	$(AR) -r libMWGateway.a $(OBJ)

clean:
	rm -f *.o libMWGateway.a

%.o: %.c
	$(CC) -I$(JDKPATH) -c -o $@ $<

%.o: %.S
	$(CC) -I$(JDKPATH) -c -o $@ $<
