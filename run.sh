
export PATH=$PATH:/usr/local/i386elfgcc/bin
export DISPLAY=:0

nasm -felf32 Boot/boot.asm -o Binaries/boot.o

# Compile Kernel:
i386-elf-gcc -c Kernel/kernel.c -o Binaries/kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
i386-elf-gcc -c Kernel/system.c -o Binaries/system.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
i386-elf-gcc -c Kernel/handler.c -o Binaries/handler.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

i386-elf-gcc -c Kernel/CPU/gdt.c -o Binaries/gdt.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
i386-elf-gcc -c Kernel/CPU/idt.c -o Binaries/idt.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
i386-elf-gcc -c Kernel/CPU/isrs.c -o Binaries/isrs.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
i386-elf-gcc -c Kernel/CPU/irq.c -o Binaries/irq.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
i386-elf-gcc -c Kernel/CPU/timer.c -o Binaries/timer.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
i386-elf-gcc -c Kernel/CPU/pci.c -o Binaries/pci.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
i386-elf-gcc -c Kernel/Memory/memory.c -o Binaries/memory.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
i386-elf-gcc -c Kernel/Filesystem/csfs.c -o Binaries/csfs.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra


i386-elf-gcc -c Kernel/Drivers/ata.c -o Binaries/ata.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
i386-elf-gcc -c Kernel/Drivers/keyboard.c -o Binaries/keyboard.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
i386-elf-gcc -c Kernel/Drivers/vga.c -o Binaries/vga.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
i386-elf-gcc -c Kernel/Drivers/double_buffer.c -o Binaries/double_buffer.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

# Fonts
i386-elf-gcc -c Fonts/font.c -o Binaries/font.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

# Shell
i386-elf-gcc -c Shell/shell.c -o Binaries/shell.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
i386-elf-gcc -c Shell/commands.c -o Binaries/commands.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

i386-elf-gcc -c Applications/pong.c -o Binaries/pong.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

i386-elf-gcc -T linker.ld -o Binaries/OS.bin -ffreestanding -O2 -nostdlib Binaries/boot.o Binaries/kernel.o\
  Binaries/system.o Binaries/gdt.o Binaries/idt.o Binaries/isrs.o Binaries/irq.o Binaries/timer.o\
  Binaries/memory.o Binaries/keyboard.o Binaries/handler.o Binaries/shell.o Binaries/commands.o\
  Binaries/vga.o Binaries/font.o Binaries/double_buffer.o Binaries/pong.o Binaries/pci.o Binaries/ata.o \
  Binaries/csfs.o -lgcc

if grub-file --is-x86-multiboot Binaries/OS.bin; then
  echo multiboot confirmed
else
  echo the file is not multiboot
fi

cp Binaries/OS.bin isodir/boot/OS.bin
cp Boot/grub.cfg isodir/boot/grub/grub.cfg
grub-mkrescue -o Binaries/caffieneOS.iso isodir

# To debug qemu:
#qemu-system-i386 --drive file=disk.img,media=disk,format=raw -cdrom Binaries/caffieneOS.iso -m 16M -boot order=dc -trace enable=ide_*
# To create disk: qemu-img create -f raw disk.img 1M

# Normal Run:
qemu-system-i386 -drive file=disk.img,media=disk,format=raw -cdrom Binaries/caffieneOS.iso -m 16M -boot order=dc

#qemu-system-i386 -m 1024 -boot d -enable-kvm -smp 3 -net nic -net user -hda disk.img -cdrom Binaries/caffieneOS.iso
#qemu-system-i386 -hda disk.img -drive file=Binaries/caffieneOS.iso,format=raw -m 16M 
#qemu-system-i386 -drive file=Binaries/caffieneOS.iso,media=disk,format=raw