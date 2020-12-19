AS=nasm
LD=ld
CC=gcc
CFLAG=-Wall -O -nostdinc -I../includes -fno-stack-protector -g -c
OBJECTS=sched.o trap.o asm.o
kernel.o:$(OBJECTS)
	$(LD) -o $@ -r $(OBJECTS)
	sync
%.o:%.c
	$(CC) -o $@ $(CFLAG) $<
%.o:%.asm
	nasm -f elf -o $@ $<
.PHONY clean:
clean:
	-rm *.o

