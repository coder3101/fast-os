#!/bin/sh

# Compile the bootsector with the build assembler
# Assuming the boot.s is in the current directory
i686-elf-as ./boot.s -o ./objects/boot.o 

# Compile the kernel.c to its object.
# Same assumptions as above
i686-elf-gcc -c ./kernel.c -o ./objects/kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

# Linker of the objects
i686-elf-gcc -T ./linker.ld -o ./fastos.bin -ffreestanding -O2 -nostdlib ./objects/boot.o ./objects/kernel.o -lgcc


