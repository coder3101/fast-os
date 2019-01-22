if grub-file --is-x86-multiboot fastos.bin; then
  echo Multiboot confirmed on this OS
else
  echo This is not Multiboot File
fi
