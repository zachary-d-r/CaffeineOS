#ifndef __FONT_H
#define __FONT_H

#define MAX_NAME_LEN  32

struct FONT {
    unsigned char sig[4];       // 'Font'
    unsigned char  height;       // height of char set
    unsigned char  max_width;    // width of widest char in set
    unsigned int info_start;   // zero based offset to the first FONT_INFO block
    signed int start;        // starting value (first entry in font == this value) (*** Signed ***)
    signed int count;        // count of chars in set ( 0 < count <= 0x10FFFF ) (*** Signed ***)
    signed int datalen;      // len of the data section in bytes
    signed int total_size;   // total size of this file in bytes
    signed int flags;        // bit 0 = fixed width font, remaining bits are reserved
    unsigned char  name[MAX_NAME_LEN]; // utf-8 null terminated
    unsigned char  resv[36];     // reserved and preserved
};

struct FONT_INFO {
    int index;   // index of this character in data section
    unsigned char width;   // Width of character
    char   deltax;  // +/- offset to print char 
    char   deltay;  // +/- offset to print char (allows for drop chars, etc)
    char   deltaw;  // +/- offset to combine with width above when moving to the next char
    unsigned char resv[4]; // reserved
};

extern unsigned char bitmap[];

#endif