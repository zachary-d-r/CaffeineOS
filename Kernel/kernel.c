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

    //initializeCSFS(addr);
    installTimer(); print("\nPIT... Done\n");
    installKeyboard(); print("Keyboard... Done\n");


    print("Done\n");

    clear();
    moveCursor(0, 0);

    
    struct csfs_entry entry;
    entry.start = 0x03;
    entry.FLAGS = 0b00000000;
    entry.name = "test";
    entry.ext = 0b10000000;

    unsigned char *c = "Hi there!\nWelcome to CaffeineOS!!\n\n\nThis is a .txt file being saved to the hard drive!! How cool is that?";

    //createFile("hello-world", c);
    //ataPioWrite(0x03, 1, "Test");
    unsigned char *r = readFile("hello-world");
    //unsigned char *r;
    //ataPioRead(0x03, 1, r);
    //unsigned char *w = readFile(r[0]);
    print(r);

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

    //init();
    //drawRectangle(10, 34, 100, 2, c);
    //drawRectangle(20, 100, 100, 50, c);

    __asm__ __volatile__ ("sti");

    for (;;);
}