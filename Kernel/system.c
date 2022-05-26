#include "system.h"

// Same as above but this time it is a 16 bit int
unsigned short *memsetw(unsigned short *dest, unsigned short val, size_t count) {
    unsigned short *temp = (unsigned short *)dest;
    for ( ; count != 0; count--) *temp++ = val;

    return dest;
}

// Return the length of a string in bytes
size_t strlen(const char *str) {
    size_t retval;
    for (retval = 0; *str != '\0'; str++) { retval++; }

    return retval;
}

// Read I/O ports to get data from devices like a keyboard
unsigned char inportb (unsigned short _port) {
    unsigned char rv;
    __asm__ __volatile__ ("inb %1, %0" : "=a" (rv) : "dN" (_port));

    return rv;
}

// Write to I/O ports to send bytes to devices
void outportb (unsigned short _port, unsigned char _data) {
    __asm__ __volatile__ ("outb %1, %0" : : "dN" (_port), "a" (_data));
}

void outportl(unsigned int port, unsigned int value) {
	   __asm__ __volatile__("outl %%eax,%%dx"::"d" (port), "a" (value));
}

void outportw (unsigned short _port, unsigned short _data) {
    __asm__ __volatile__ ("outw %1, %0" : : "dN" (_port), "a" (_data));
}

unsigned short inportw (unsigned short port) {
    unsigned short result;
    __asm__("in %%dx, %%ax" : "=a" (result) : "d" (port));
    return result;
}

// Append a char to a string
void append(char *str, char c) {
    int len = strlen(str);
    str[len] = c;
    str[len+1] = '\0';
}


// Return < 0 if s1<s2, 0 if s1 == s2, and > 0 if s1 > s2
int strcmp(char s1[], char s2[]) {
    int i;
    for (i = 0; s1[i] == s2[i]; i++) { if (s1[i] == 0) { return 0; } }
    return s1[i] - s2[i];
}

// Remove the last element of a list
void remove(char *str) {
    str[strlen(str)-1] = 0;
}

// Turn a string into an integer
int strToInt(char *str) {
    int num = 0;
    for (int i = 0; i < strlen(str); i++) {
        if (str[i] != ' ') {
            char currentNum = str[i]-0x30;
            num = (num * 10) + currentNum;
        }
    }
    return num;
}

char* inttostr(int i)
{
      static char text[12];
      int loc = 11;
      text[11] = 0;
      char neg = 1;
      if (i >= 0)
      {
         neg = 0;
         i = -i;
      }
      while (i)
      {
          text[--loc] = '0' - (i%10);
          i/=10;
      }
      if (loc==11)
          text[--loc] = '0';
      if (neg)
         text[--loc] = '-';      
      return &text[loc];
}

int pow(int base, int exp)
    {
      if(exp < 0)
        return -1;

        int result = 1;
        while (exp)
        {
            if (exp & 1)
                result *= base;
            exp >>= 1;
            base *= base;
        }

        return result;
    }