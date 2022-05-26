#include "../system.h"

// Defines a GDT entry. The packed at the end prevents the compiler from "optimizing" the package
struct gdtEntry {
    unsigned short limitLow;
    unsigned short baseLow;
    unsigned char baseMiddle;
    unsigned char access;
    unsigned char granularity;
    unsigned char baseHigh;
} __attribute__((packed));

// Specitsl pointer that includes the limit, The max bytes taken up by the GDT minus 1
struct gdtPtr {
    unsigned short limit;
    unsigned int base;
} __attribute__((packed));

struct gdtEntry gdt[3];  // Our gdt with 3 entries
struct gdtPtr gp;  // Set up our GDT pointer

extern void flushGdt();  // This is defined in boot.s

// Set up descriptor in the GDT
void setGdtGate(int num, unsigned long base, unsigned long limit, unsigned char access, unsigned char gran) {
    // Set descriptor base address
    gdt[num].baseLow = (base & 0xFFFF);
    gdt[num].baseMiddle = (base >> 16) & 0xFF;
    gdt[num].baseHigh = (base >> 24) & 0xFF;

    // Set descriptor limits
    gdt[num].limitLow = (limit & 0xFFFF);
    gdt[num].granularity = ((limit >> 16) & 0x0F);

    // Set up granularity and acces flags
    gdt[num].granularity |= (gran & 0xF0);
    gdt[num].access = access;
}

// Install the GDT by setting up the 3 entries and then tell the tell the proccesser where the GDT is and update the regesters
void installGdt() {
    gp.limit = (sizeof(struct gdtEntry) * 3) - 1;  // Set up the GDT limit
    gp.base = &gdt;  // Set up GDT pointer

    setGdtGate(0, 0, 0, 0, 0);  // Our NULL descriptor
    setGdtGate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);  // The code segment descriptor
    setGdtGate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);  // The data segment descriptor

    flushGdt();  // Install the new changes
}