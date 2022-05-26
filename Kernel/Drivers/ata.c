#include "ata.h"
#include "../system.h"
#include "vga.h"
#include "../multiboot.h"

// Source: https://github.com/knusbaum/kernel/blob/master/ata_pio_drv.c

/*
 * To use the IDENTIFY command, select a target drive by sending 0xA0 for the master drive,
 * or 0xB0 for the slave, to the "drive select" IO port. On the Primary bus, this would be port 0x1F6.
 * Then set the Sectorcount, LBAlo, LBAmid, and LBAhi IO ports to 0 (port 0x1F2 to 0x1F5). Then send
 * the IDENTIFY command (0xEC) to the Command IO port (0x1F7).
 *
 * Then read the Status port (0x1F7) again.
 * If the value read is 0, the drive does not exist.
 * For any other value: poll the Status port (0x1F7) until bit 7 (BSY, value = 0x80) clears.
 * Because of some ATAPI drives that do not follow spec, at this point you need to check the
 * LBAmid and LBAhi ports (0x1F4 and 0x1F5) to see if they are non-zero. If so, the drive is not ATA,
 * and you should stop polling. Otherwise, continue polling one of the Status
 * ports until bit 3 (DRQ, value = 8) sets, or until bit 0 (ERR, value = 1) sets.
 *
 * At that point, if ERR is clear, the data is ready to read from the Data port (0x1F0).
 * Read 256 16-bit values, and store them.
 */

unsigned char status;
unsigned int bootDevice;

unsigned char detectAta() {

    // Clear everything
    inportb(ATA_PRIMARY_COMM_REGSTAT);
    outportb(ATA_PRIMARY_DRIVE_HEAD, 0xA0);

    inportb(ATA_PRIMARY_COMM_REGSTAT);
    outportb(ATA_PRIMARY_SECCOUNT, 0);

    inportb(ATA_PRIMARY_COMM_REGSTAT);
    outportb(ATA_PRIMARY_LBA_LO, 0);

    inportb(ATA_PRIMARY_COMM_REGSTAT);
    outportb(ATA_PRIMARY_LBA_MID, 0);

    inportb(ATA_PRIMARY_COMM_REGSTAT);
    outportb(ATA_PRIMARY_LBA_HI, 0);

    // Identify command
    inportb(ATA_PRIMARY_COMM_REGSTAT);
    outportb(ATA_PRIMARY_COMM_REGSTAT, 0xEC);
    outportb(ATA_PRIMARY_COMM_REGSTAT, 0xE7);

    status = inportb(ATA_PRIMARY_COMM_REGSTAT);  // Read the status port, if it is 0, the dirve does not exist.

    print("Waiting for status...\n");
    print("\n");
    while (!status & STAT_BSY) {
        unsigned int i = 0;

        for(i = 0; i < 0x0FFFFFFF; i++) {}
        status = inportb(ATA_PRIMARY_COMM_REGSTAT);
    }

    if (status == 0) { print("Status = 0, No drive was found"); return 0; }
    print("Status:"); print((inttostr(status))); print(" Indicates the existance of a drive. Polling while STAT_BSY... ");

    while (status & STAT_BSY) { status = inportb(ATA_PRIMARY_COMM_REGSTAT); }  // Wait until the status bit clears
    print("Done.\n");

    unsigned char mid = inportb(ATA_PRIMARY_LBA_MID);
    unsigned char hi = inportb(ATA_PRIMARY_LBA_HI);
    if (mid || hi) { print("The drive is not ATA."); return 0; }

    print("Waiting for ERR or DRQ... ");
    while (!(status & (STAT_ERR | STAT_DRQ))) {
        status = inportb(ATA_PRIMARY_COMM_REGSTAT);
    }
    print("Done.\n");

    if (status & STAT_ERR) {
        // There was an error on the drive. Forget about it.
        print("ERROR IN DRIVE.");
        return 0;
    }

    print("Reading IDENTIFY structure... ");
    unsigned int buffer[256*2];
    for (unsigned int i = 0; i < 256; i++) { buffer[i] = inportb(0xF0); }
    print("Done.\n");

    print("\nSuccess! Disk ready to go!");
    return 1;
}

/*
* Send 0x40 for the "master" or 0x50 for the "slave" to port 0x1F6: outb(0x1F6, 0x40 | (slavebit >> 4))
* outb (0x1F2, sectorcount high byte)
* outb (0x1F3, LBA4)
* outb (0x1F4, LBA5)
* outb (0x1F5, LBA6)
* outb (0x1F2, sectorcount low byte)
* outb (0x1F3, LBA1)
* outb (0x1F4, LBA2)
* outb (0x1F5, LBA3)
* Send the "READ SECTORS EXT" command (0x24) to port 0x1F7: outb(0x1F7, 0x24)
*/
void ataPioRead(unsigned long long LBA, unsigned short sectorcount, unsigned char *target) {
    outportb(ATA_PRIMARY_DRIVE_HEAD, 0x40); // Select master
    outportb(ATA_PRIMARY_SECCOUNT, (sectorcount >> 8) & 0xFF);  // Sector Count hi
    outportb(ATA_PRIMARY_LBA_LO, (LBA >> 24) & 0xFF); // LBA4
    outportb(ATA_PRIMARY_LBA_MID, (LBA >> 32) & 0xFF);  //LBA5
    outportb(ATA_PRIMARY_LBA_HI, (LBA >> 40) & 0xFF);  // LBA6

    outportb(ATA_PRIMARY_SECCOUNT, sectorcount & 0xFF); // Sector Count lo
    outportb(ATA_PRIMARY_LBA_LO, LBA & 0xFF); // LBA1
    outportb(ATA_PRIMARY_LBA_MID, (LBA >> 8) & 0xFF);  //LBA2
    outportb(ATA_PRIMARY_LBA_HI, (LBA >> 16) & 0xFF);  // LBA3
    outportb(ATA_PRIMARY_COMM_REGSTAT, 0x24);  // READ SECTORS EXT command

    for (unsigned int i = 0; i < sectorcount; i++) {
        pollATA();  // Wait for it to be able to transfer data

        // Transfer the data
        for (int j = 0; j < 256; j++) { target[j] = inportw(ATA_PRIMARY_DATA); }
        target += 512;
    }
}

void ataPioWrite(unsigned long long LBA, unsigned short sectorcount, unsigned char *target) {
    outportb(ATA_PRIMARY_DRIVE_HEAD, 0x40); // Select master
    outportb(ATA_PRIMARY_SECCOUNT, (sectorcount >> 8) & 0xFF);  // Sector Count hi
    outportb(ATA_PRIMARY_LBA_LO, (LBA >> 24) & 0xFF); // LBA4
    outportb(ATA_PRIMARY_LBA_MID, (LBA >> 32) & 0xFF);  //LBA5
    outportb(ATA_PRIMARY_LBA_HI, (LBA >> 40) & 0xFF);  // LBA6

    outportb(ATA_PRIMARY_SECCOUNT, sectorcount & 0xFF); // Sector Count low
    outportb(ATA_PRIMARY_LBA_LO, LBA & 0xFF); // LBA1
    outportb(ATA_PRIMARY_LBA_MID, (LBA >> 8) & 0xFF);  //LBA2
    outportb(ATA_PRIMARY_LBA_HI, (LBA >> 16) & 0xFF);  // LBA3
    outportb(ATA_PRIMARY_COMM_REGSTAT, 0x34);  // WRITE SECTORS EXT command

    for (unsigned char i = 0; i < sectorcount; i++) {
        pollATA();  // Wait for it to be able to transfer data

        // Transfer the data
        for (int j = 0; j < 256; j++) { outportw(ATA_PRIMARY_DATA, target[j]); }
        target += 512;
    }

    // Flush the cache.
    outportb(ATA_PRIMARY_COMM_REGSTAT, 0xE7);
    // Poll for BSY.
    while (inportb(ATA_PRIMARY_COMM_REGSTAT) & STAT_BSY) {}
}

void pollATA() {
    while (1) {
        unsigned char status = inportb(ATA_PRIMARY_COMM_REGSTAT);
        if (status & STAT_DRQ) { break; }  // Drive is ready to transfer data!
        else if(status & STAT_ERR) { print("DISK SET ERROR STATUS!"); }
}
}

unsigned int getBootDevice() { return bootDevice; }

void initATA(unsigned long addr) {
    multiboot_info_t *mbiInfo = (multiboot_info_t *)addr;
    bootDevice = (mbiInfo->boot_device >> 24) & 0xff;

    detectAta();
    unsigned int *temp;
    ataPioRead(0x01, 1, temp);
}