#include "../system.h"
#include "../multiboot.h"
#include "../Drivers/ata.h"
#include "csfs.h"

multiboot_info_t *mbiMemFs;
struct csfs_header header;

void initializeCSFS(unsigned long addr) {
    mbiMemFs = (multiboot_info_t *)addr;

    // Read what is currently in the header
    unsigned int *readHeader;
    ataPioRead(0x01, 2, readHeader);

    // Create the header and put it into memory
    if (readHeader[0x0] != 0x0D || readHeader[0x02] != 0x19) {
        unsigned int *newHeader;

        // Signed Bytes
        newHeader[0] = 0x0D;
        newHeader[1] = 0x19;

        newHeader[2] = pow(10, 9);  // Disk Size

        newHeader[3] = 0x01 + 0x0C;  // LBA start location/cluster start/stream start

        newHeader[4] = 0xFF;  // Bytes per LBA
        newHeader[5] = 0xFF; // LBAs per cluster

        newHeader[6] = 0x01; // Version number


        ataPioWrite(0x01, 2, newHeader);
        ataPioRead(0x01, 2, readHeader);

        // Create Root Directory
        struct csfs_entry root;
        root.FLAGS = 0b00100000;
        root.name = "root";
        root.ext = 0b00000000;
        root.directoryLocation = 0x1C + 0x01;
        root.fileSize = 0x0;
        root.clusterLocation = 0x1C + 0x01;
        root.nextEntry = 0x1C + 0x01;

        createFile(root);

        struct csfs_entry initTXT;
        initTXT.FLAGS = 0b00000000;
        initTXT.name = "root";
        initTXT.ext = 0b10000000;
        initTXT.directoryLocation = 0x1C + 0x01;
        initTXT.fileSize = 0x0;
        initTXT.clusterLocation = getFreeLBA();
        initTXT.nextEntry = 0x0D;

        createFile(initTXT);

    }

    // Store data in header
    header.diskSize = readHeader[2];
    header.streamLocation = readHeader[3];
    header.bytesPerLBA = readHeader[4];
    header.LBAPerCluster = readHeader[5];
    header.versionNumber = readHeader[6];

}

int getFreeLBA() {
    unsigned short *endByte = 0x0D;
    unsigned long long *t;
    int i = 0;

    while (*endByte == 0x0D) {
        ataPioRead(0x0C + i, 1, t);
        for (int j = 0; j < strlen(t); j++) {
            if (j % 27 == 0) { *endByte = t[i]; }
            if (*endByte != 0x0D) { break; }
        }
        i++;
    }
    return header.streamLocation + i + 1;

}

void createFile(struct csfs_entry entry) {
    unsigned long long *file;
    unsigned long long *r;
    int i;

    ataPioRead(getFreeLBA()-1, 1, r);
    for (i = 0; i < strlen(r); i++) {
        if (r[i] % 27 != 0x0D) {break;}
        file[i] = r[i];
    }
    if (strlen(file) >= 255) { memcpy(file, 0, strlen(file)); i = 0; }

    file[0+i] = entry.start;
    file[1+i] = entry.FLAGS;
    file[2+i] = entry.name;
    file[3+i] = entry.ext;
    file[4+i] = entry.directoryLocation;
    file[5+i] = entry.fileSize;
    file[6+i] = entry.clusterLocation;
    file[7+i] = entry.nextEntry;

    if (i > 0) { ataPioWrite(getFreeLBA()-1, 1, file); }
    else { if (i > 0) { ataPioWrite(getFreeLBA(), 1, file); } }
}

unsigned char *readFile(unsigned char *filename) {
    unsigned long long *r;
    unsigned char *currentName = "NULL";
    struct csfs_entry file;
    int i;
    int offset = 0;

    while (currentName != filename) {
        ataPioRead(header.streamLocation+offset, 1, r);

        for (i = 0; i < 256/30; i++) {
            currentName = r[(i * 29) + 2];
            if (currentName == filename) { break; }
            
        }

        offset++;
    }

    unsigned int index = i + 30;

    return currentName;
    
}