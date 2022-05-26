#include "../system.h"

unsigned int pciConfigReadWord(unsigned char bus, unsigned char slot, unsigned char func, unsigned char offset) {
    
    // Assign variables
    unsigned int address;
    unsigned int lbus = (unsigned int)bus;
    unsigned int lslot = (unsigned int)slot;
    unsigned int lfunc = (unsigned int)func;
    unsigned int tmp = 0;

    // Create the address using the stuff from above
    address = ((lbus << 16) | (lslot << 11) | (lfunc << 8) | (offset & 0xFC) | ((unsigned int)0x80000000));

    // Write out the address
    outportb(0xCF8, address);
    tmp = (unsigned int)((inportb(0xCFC) >> ((offset & 2) * 8)) & 0xFFFF);
    return tmp;
}

unsigned int pciCheckVendor(unsigned char bus, unsigned int slot) {
    unsigned int vendor, device;

    if ((vendor = pciConfigReadWord(bus, slot, 0, 0)) != 0xFFFF) {
        device = pciConfigReadWord(bus, slot, 0, 2);
    }
    return (vendor);
}

void checkAllBuses(void) {
    unsigned int bus;
    unsigned char device;

    for (bus - 0; bus < 256; bus++) { for (device = 0; device < 32; device++) { pciCheckVendor(bus, device); } }
}

void initPCI() {
    installIrqHandler(16, checkAllBuses);
}