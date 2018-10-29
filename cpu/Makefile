CC=gcc
BASEFLAGS := -Wall -Werror -MMD -MP -g -fPIC -O0
CFLAGS := $(BASEFLAGS) -ffreestanding -nostdlib -nostdinc -fno-builtin -mno-red-zone -fno-stack-protector
CFLAGS += -I. -I ../ -I ../../c/include -std=gnu11
OUTPUT=cpu
SRC=./cpu.c ./main.c ../utils.c


all: $(OUTPUT)

litc.o: ../../c/litc.c
	$(CC) $(CFLAGS) ../../c/litc.c -O -c -o litc.o

$(OUTPUT): litc.o $(SRC)
	$(CC) $(CFLAGS) $(SRC) litc.o -Wl,-T ../../c/linker.ld -Wl,--build-id=none -o $(OUTPUT)
