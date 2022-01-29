
#UBUNTU TEST
#add 32-bit gcc cross-compiler to your $PATH variable


#ASSEMBLER COMPILATION
nasm -fbin ./boot/boot.asm 
nasm -fbin ./boot/locateFS.asm
nasm -fbin ./boot/kernelLoader.asm


cd ./handlers
nasm -f elf32 keyboard.asm -o keyboard.o
cd ..

cd ./syscall
nasm -f elf32 syscall.asm -o syscall.o
cd ..

cd ./pit
nasm -f elf32 pit.asm -o pit.o
cd ..



#COMPILE																										 HERE YOU MUST PUT PATH TO SRC DIRECTORY
i386-elf-gcc -O0 -masm=intel -Wno-builtin-declaration-mismatch -nostdlib -ffreestanding -Wno-int-conversion -Wno-unused-function -I /home/witut/Desktop/xaninOS/src kernel.c ./handlers/keyboard.o ./pit/pit.o ./syscall/syscall.o -o kernel.bin

#dd if=/dev/zero of=zero.img bs=512 count=2024

cat ./boot/boot ./boot/kernelLoader kernel.bin ./boot/locateFS home.tar > xanin.bin



#MOV XANIN.img to BIN


dd if=xanin.bin of=xanin.img
mv xanin.img -f ../bin
mv xanin.bin -f ../bin
cd ../bin
make
./detectUstar

cp xanin.img xanin.bin

cd ../src

qemu-system-x86_64 -machine accel=kvm:tcg -nic none -m 512 -usb ../bin/xanin.bin


