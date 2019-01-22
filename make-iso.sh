mkdir -p isodir/boot/grub
cp fastos.bin isodir/boot/fastos.bin
cp grub.cfg isodir/boot/grub/grub.cfg
grub-mkrescue -o fastos.iso isodir
