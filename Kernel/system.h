#ifndef __SYSTEM_H
#define __SYSTEM_H

typedef int size_t;

// This defines what the stack looks like after an ISR was running 
struct regs {
    unsigned int gs, fs, es, ds;  // pushed the segs last 
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;  // pushed by 'pusha' 
    unsigned int int_no, err_code;  // our 'push byte #' and ecodes do this 
    unsigned int eip, cs, eflags, useresp, ss;  // pushed by the processor automatically 
};

// System.c
extern unsigned short *memsetw(unsigned short *dest, unsigned short val, size_t count);
extern size_t strlen(const char *str);
extern unsigned char inportb (unsigned short _port);
extern void outportb (unsigned short _port, unsigned char _data);
extern void append(char *str, char c);
extern int strcmp(char s1[], char s2[]);
extern void remove(char *str);
extern int strToInt(char *str);
extern char* inttostr( int zahl );
extern void outportl(unsigned int port, unsigned int value);
extern unsigned short inportw (unsigned short port);
extern void outportsw(unsigned short port, void *addr, unsigned int count);
extern void outportw (unsigned short _port, unsigned short _data);
extern int pow(int base, int exp);

// memory.c
extern unsigned char *memcpy(unsigned char *dest, const unsigned char *src, int count);
extern void *memset(void *dest, char val, size_t count);
extern void initializeMemory(unsigned long addr, char startAddr, char endAddr);
extern void* kmalloc(int size);
extern void memcut(int size);
extern void memreset();

// screen.c
// extern void updateCursor(void);
// extern void moveCursor(int x, int y);
// extern void clear();
// extern void putch(unsigned char c);
// extern void print(char *str);
// extern void setTextColor(unsigned char foreground, unsigned char background);
// extern void initVideo(void);
// extern void moveCursorRealative(int x, int y);
// extern void putchKeyboard(unsigned char c, int shiftPressed);
// extern int getCursorX();
// extern int getCursorY();
// extern char getCharacterAtCursor();
// extern void printInt(int number);
// extern int getForeground();
// extern int getBackground();
// void plotVerticalLine(int startY, int endY, int x, unsigned char color);

// gdt.c
extern void setGdtGate(int num, unsigned long base, unsigned long limit, unsigned char access, unsigned char gran);
extern void installGdt();

// idt.c
extern void setIdtGate(unsigned char num, unsigned long base, unsigned short sel, unsigned char flags);
extern void installIdt();

// isrs.c
void installIsrs();

// irq.c
extern void installIrqHandler(int irq, void (*handler)(struct regs *r));
extern void uninstallIrqHandler(int irq);
extern void installIrqs();

// pci.c
extern void initPCI();
extern void checkAllBuses(void);

// timer.c
extern void installTimer();
extern void timerPhase(int hz);
extern void waitHz(int ticks);
extern void wait(int ms);

// keyboard.c
extern void installKeyboard();

// mouse.c
extern void installMouse();

// handler.c
extern void mainHandler(char key);
extern int getStatus(char *name);
extern void changeProcess(char *name);
extern void keyboardControll(char key);
extern void init();

#endif