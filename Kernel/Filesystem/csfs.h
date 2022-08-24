#ifndef __CSFS_H
#define __CSFS_H

extern void createFile(unsigned char *name, unsigned char *content);
extern unsigned char *readFile(unsigned char *filename);

struct csfs_entry {
    unsigned char start;
    unsigned short FLAGS;
    unsigned char *name; 
    unsigned char ext;
    unsigned short directoryLocation;
    unsigned int fileSize;
    unsigned short clusterLocation;
    unsigned short nextEntry;

} __attribute__ ((packed)) ;


// struct csfs_header {
//     unsigned char *signedBits;
//     unsigned int diskSize;
//     unsigned short *streamLocation;
//     unsigned char bytesPerLBA;
//     unsigned short LBAPerCluster;
//     unsigned char versionNumber;

// } __attribute__ ((packed));

// struct csfs_entry {
//     unsigned char start;
//     unsigned short FLAGS;
//     unsigned char *name; 
//     unsigned char ext;
//     unsigned short directoryLocation;
//     unsigned int fileSize;
//     unsigned short clusterLocation;
//     unsigned short nextEntry;

// } __attribute__ ((packed)) ;


// extern void initializeCSFS(unsigned long addr);
// extern unsigned char *readFile(unsigned char *filename);
// extern int getFreeLBA();

#endif