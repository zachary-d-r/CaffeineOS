#include "../system.h"

// Define an IDT entrtry
struct idtEntry {
    unsigned short baseLo;
    unsigned short sel;  // Kernel segment goes here
    unsigned char always0;  // This is always 0
    unsigned char flags;  // Set using table: http://www.osdever.net/bkerndev/Docs/idt.htm
    unsigned short baseHi;
} __attribute__((packed));

// Special pointer with limit
struct idtPtr {
    unsigned short limit;
    unsigned int base;
} __attribute__((packed));

struct idtEntry idt[256];  // Define the first 256 idt entries
struct idtPtr idtp;  // The idt pointer object

extern void loadIdt();  // Defined in boot.asm

// Set an entry in the IDT
void setIdtGate(unsigned char num, unsigned long base, unsigned short sel, unsigned char flags) {
    // Interrupt routine's base address
    idt[num].baseLo = (base & 0xFFFF);
    idt[num].baseHi = (base >> 16) & 0xFFFF;

    // The segment or 'selector' this idt entry will use and flags
    idt[num].sel = sel;
    idt[num].always0 = 0;
    idt[num].flags = flags;
}

// Install the IDT
void installIdt() {
    // Sets the special IDT pointer up, just like in 'gdt.c'
    idtp.limit = (sizeof (struct idtEntry) * 256) - 1;
    idtp.base = &idt;

    // Clear out the idt by making it all zeros
    memset(&idt, 0, sizeof(struct idtEntry) * 256);

    // ISR's go here

    loadIdt();  // Point the processor's internal register to the new IDT
}