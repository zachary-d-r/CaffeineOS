#include "../system.h"
#include "../multiboot.h"
#include "vga.h"
#include "../../Fonts/font.h"

// The screen is 80x30

unsigned char *framebuffer;
multiboot_info_t *mbi;
int cursorX, cursorY = 0;  // Cursor positions like text mode to make printing characters to the screen easier
unsigned int fg, bg;

void initializeVGA(unsigned long addr) {
    /*
    Initialize the VGA driver by getting the multiboot header information and the framebuffer address.
    */

    mbi = (multiboot_info_t *)addr;  // Get the multiboot information from the address
    framebuffer = (unsigned char *)mbi->framebuffer_addr;  // Get the framebuffer address
}

// Scrolls the screen when needed
void scroll(void) {
    unsigned blank, temp;  // A space and where we index video memory

    blank = 0x045ce0 & 0xff;

    // If we are at the last rowm scroll up
    if (cursorY >= 30*16) {
        // Move current chunk that makes up the screen back in the buffer by one line
        temp = 16;
        memcpy(framebuffer, framebuffer + temp*4 * 80*8, (((30*16 + temp) * 80*8) * 4));

        // Then we set the last line to the blank character
        // Plot the pixels manually
        for (int i = 0; i <= 16; i++) {
            for (int j = 0; j <= 80*8; j++) { plotPixle(j, (30*16)-i, bg); }
    }

        cursorY = 30*16 - 16;
    }
    /*else if (cursorY < 0) { 
        for (int i = 160 * 2; i < 4000 - 160; i++) *(videoMemory + i - 160) = blank;
        moveCursorRealative(0, 1);
     }*/
}

void plotPixle(int x, int y, unsigned int color) {
    /*
    Plot a pixle in a certain color to the screen

    Parameters
    ----------
        x : int
            The x position of the pixle
        y : int
            The y position of the pixle
        color : unsigned int
            The color inputted in hexadecimal (BGR)
    */

    switch (mbi->framebuffer_bpp) {
    case 8:
        framebuffer[mbi->framebuffer_pitch * y + 1 * x] = 4; 
        break;
    case 15:
        break;
    case 16:
        framebuffer[mbi->framebuffer_pitch * y + 2 * x + 0] = color;
        break;
    case 24:
        framebuffer[mbi->framebuffer_pitch * y + 3 * x + 0] = color;
        break;
    case 32:
        framebuffer[mbi->framebuffer_pitch * y + 4 * x + 0] = color & 0xff;
        framebuffer[mbi->framebuffer_pitch * y + 4 * x + 1] = (color >> 8) & 0xff;
        framebuffer[mbi->framebuffer_pitch * y + 4 * x + 2] = (color >> 16) & 0xff;
        break;
    default:
        // error
        break;
    }
}

void plotPixleBuffer(int x, int y, unsigned int color, unsigned char *buffer) {
    /*
    Plot a pixle in a certain color to the screen

    Parameters
    ----------
        x : int
            The x position of the pixle
        y : int
            The y position of the pixle
        color : unsigned int
            The color inputted in hexadecimal (BGR)
    */

    switch (mbi->framebuffer_bpp) {
    case 8:
        buffer[mbi->framebuffer_pitch * y + 1 * x] = 4; 
        break;
    case 15:
        break;
    case 16:
        buffer[mbi->framebuffer_pitch * y + 2 * x + 0] = color;
        break;
    case 24:
        buffer[mbi->framebuffer_pitch * y + 3 * x + 0] = color;
        break;
    case 32:
        buffer[mbi->framebuffer_pitch * y + 4 * x + 0] = color & 0xff;
        buffer[mbi->framebuffer_pitch * y + 4 * x + 1] = (color >> 8) & 0xff;
        buffer[mbi->framebuffer_pitch * y + 4 * x + 2] = (color >> 16) & 0xff;
        break;
    default:
        // error
        break;
    }
}

int getPitch() {
    return mbi->framebuffer_pitch;
}

void plotLine(int x1, int y1, int x2, int y2, unsigned int color) {
    /*
    Plot a line from (x1, y1) to (x2, y2) in a certain color.
    */

    float slope;

    if (x2 - x1 != 0) { 
        slope = (y2 - y1)/(x2 - x1);  // Calculate the slope of the line 

        // Plot the line
        for (int i = x1; i < x2; i++) {
            int y = slope * (i - x1) + y1;
            plotPixle(i, y, color);
        }
    } 
    // Be able to plot vertical lines
    else {
        for (int i = y1; i < y2; i++) { plotPixle(x1, i, color); }
    } 

}

void drawRectangle(int x, int y, int width, int height, unsigned int c) {

    plotLine(x, y, x+width, y, c);
    plotLine(x, y+height, x+width, y+height, c);
    plotLine(x, y, x, y+height, c);
    plotLine(x+width, y, x+width, y+height, c);

    for (int i = x; i < width+x; i++) { plotLine(i, y, i, y+height, c); }
}

void putchVGA(char character, int x, int y, unsigned int color) {
    int cx, cy;
    unsigned char *glyph = bitmap + (character * 14);

    for (cy = 0; cy < 14; cy++) {
        for (cx = 0; cx < 8; cx++) {
            if (glyph[cy] & (1 << (7-cx))) { plotPixle(x + cx, y + cy, color); }
            //plotPixle(defaultFont[cy]&mask[cx]?12:0,x+cx,y+cy-12);
        }
    }
    scroll();
}

void putchVGAIndex(char index, int x, int y, unsigned int color) {
    int cx, cy;
    unsigned char *glyph = bitmap + (index);

    for (cy = 0; cy < 14; cy++) {
        for (cx = 0; cx < 8; cx++) {
            if (glyph[cy] & (1 << (7-cx))) { plotPixle(x + cx, y + cy, color); }
            //plotPixle(defaultFont[cy]&mask[cx]?12:0,x+cx,y+cy-12);
        }
    }
    scroll();
}

void moveCursor(int x, int y) {
    cursorX = x;
    cursorY = y;
}

void moveCursorRealative(int x, int y) {
    moveCursor(cursorX + x, cursorY + y);
}

int getCursorX() { return cursorX; }
int getCursorY() { return cursorY; }

void putch(char c) {

    if (c == 0x08) { if (cursorX >= 9) { cursorX = cursorX - 8; clearChar(); } }  // Handle a backspace
    else if (c == 0x09) { cursorX = (cursorX + 8*8) & ~(8 - 1); }  // Handle a tab by incrementing the cursor's x, but only to a point that will make it divisable by 8
    else if (c == '\r') { cursorX = 0; }  // Handle a carrige return
    else if (c == '\n') { cursorX = 0; cursorY = cursorY + 16; }  // Handle a newline

    // Any character > a space is printable
    else if (c >= ' ') {
        putchVGA(c, cursorX, cursorY, fg);
        cursorX = cursorX + 8;  // Increment the cursor by 8
    }

    if (cursorX >= 80*8) { cursorX = 0; cursorY = cursorY + 16; }  // Insert a new line if the cursor has hit the edge of the screen

    moveCursor(cursorX, cursorY);
    scroll();
}

void print(char *str) {
    int i;  // An iterator
    for (i = 0; i < strlen(str); i++) { putch(str[i]); }
}

void clear() {
    for (int i = 0; i <= 480; i++) {
        for (int j = 0; j <= 80*8; j++) { plotPixle(j, i, bg); }
    }
}

// Clear the char in front of the cursor
void clearChar() {
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 8; j++) { plotPixle(cursorX + j, cursorY + i, bg); }
    }
}

// Change the background and foreground color
void setColor(unsigned int foreground, unsigned int background) {
    fg = foreground;
    bg = background;
}

int getBackground() { return bg; }
int getForeground() { return fg; }