# Fast OS

 

A very simple operating system that just boots up very quickly and hence `fastos`. This is a bare bone kernel which only knows how to write stuffs to video memory. If you want to know how things work, it may be a good to have a look at code. 

 

## Components 

| File(s)                   | What it does ?                                               |
| ------------------------- | ------------------------------------------------------------ |
| `isodir`                  | This contains the kernel and the grub configuration. This section is used to build the `iso` using `grub-mkrescue` |
| `objects`                 | This directory has two objects. Each compiled from `boot.s` and `kernel.c`. The linker links the two to produce our kernel. |
| `boot.s`                  | It Assembly Code well documented. This will be invoked by BIOS on startup and its job is to load the high level kernel and do some initializations. |
| `build.sh`                | A script that builds the kernel image and outputs `fastos.bin`, it is the kernel as ELF binary. |
| `fastos.bin`              | The Kernel itself.                                           |
| `fastos.iso`              | A CD-ROM bootable iso that can boot with GRUB 2. It contains the kernel and grub configuration. It will be build by the contents inside of `isodir`. |
| `grub.cnf`                | The Grub config file that adds the entry for the OS to boot. |
| `kernel.c`                | This is the kernel source itself.                            |
| `linker.ld`               | This is the linker script. It will link the two objects in `/objects` to produce `fastos.bin` (the kernel). |
| `make-iso.sh`             | This script makes the `iso` using the contents of `isodir`   |
| `multiboot-sanity.sh`     | This is a simple script that checks if the kernel is multibootable by the grub or not. It must be bootable |
| `run.sh` and `run_iso.sh` | This runs the kernel in `qemu-system-i386` emulator. ISO or non iso whichever you prefer. |



## Booting the OS

In order to boot the os you can either make a bootable cdrom using the iso or you can boot the kernel directly on a `qemu-system-i386` by running the `run.sh `. Make sure you have the qemu emulator installed.



### OS in Action



![GRUB SCREEN](https://i.imgur.com/FUnAdz8.png)

Grub Boot Screen.



![Boot](https://i.imgur.com/nJ7zTjb.png)

Information VGA Display

## How to Compile

Building an OS requires a cross-compiler. The one that is provided by your OS will run but is likely to throw many error which will be difficult to debug. In order to compile the codes you need to build a `gcc-7.3.0` and `binutils-2.3` that targets `i686-elf` . Google on how to build cross-compilers and then use those compilers to run the following scripts. (Make sure to add the new gcc and binutils to path).

* `build.sh` : To build the kernel
* `multiboot-sanity.sh` : It must return, "Multibootable OS"
* `make-iso` : To build the ISO



Any hacks that you want to do you can do it on `kernel.c` and it will be good to not mess the file you don't have any idea. This `kernel.c` can also not include stuffs like `stdio.h` or other system header. Our OS does not provide these headers as of now. 



---

