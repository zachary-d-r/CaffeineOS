#include "system.h"
#include "Drivers/vga.h"
#include "../Applications/applications.h"
#include "Drivers/double_buffer.h"
#include "Drivers/ata.h"
#include "Filesystem/csfs.h"
#include "multiboot.h"

void main(unsigned long addr) {
    
    extern char _bootstrap_start[];
    extern char _bootstrap_end[];
    
    installGdt();
    installIdt();
    installIsrs();
    installIrqs();

    initializeMemory(addr, _bootstrap_start, _bootstrap_end);


    //initPCI();
    
    initializeVGA(addr);
    initDoubleBuffer();
    setColor(0xffffff, 0x045ce0);
    clear();
    print("GDT, IDT, ISRs, IRQs, MEMORY Succesfully Initialized\n");
    print("Initialize VGA...");
    print("Done\n");

    print("Initializing Drivers...\n");
    initATA(addr); print("ATA... Done\n");

    char *bwrite[512];
    for(int i = 0; i < 512; i++) {
       bwrite[i] = 0x0F;
    }

    for (int k = 0; k < 12; k++) {ataPioWrite(0x00+k, 2, bwrite);}

    initializeCSFS(addr);
    installTimer(); print("\nPIT... Done\n");
    installKeyboard(); print("Keyboard... Done\n");


    print("Done\n");

    clear();
    moveCursor(0, 0);


    // unsigned long long *r;
    // int l = getFreeLBA();
    // ataPioRead(0x02 + 0x0C, 1, r);

    // print("NAME: "); for (int i = 0; i < 256; i++) { print(inttostr(r)); }

    //loadFile("Test", "root", "txt", "Test");
    //unsigned char *er = readFile("rdg");

    //print(er);

    // unsigned int lba = 0x04;

    // unsigned int *target;
    
    // ataPioRead(lba, 1, target);

    // print("\nRead succecfull!\n");
    // for (int i = 0; i < strlen(target); i++) {
    //     print(inttostr(target[i] & 0xFF));
    //     print(" "); 
    // }
    
    // print("\n\nWrite Succesfull!\n");

    // ataPioRead(lba, 1, target);

    // for (int i = 0; i < 64; i++) {
    //    print(inttostr(target[i] & 0xFF));
    //    print(" "); 
    // }
    

    // int h = 20;
    // plotPixleBuffer(3, h, 0xffffff, framebuffer);int a = (the_int >> 24) & 0xff
    // plotPixleBuffer(4, h, 0xffffff, framebuffer);
    // plotPixleBuffer(5, h, 0xffffff, framebuffer);

    //printInt(1);

    init();
    //drawRectangle(10, 34, 100, 2, c);
    //drawRectangle(20, 100, 100, 50, c);

    __asm__ __volatile__ ("sti");

    for (;;);
}