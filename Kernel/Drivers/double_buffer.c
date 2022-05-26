#include "../system.h"
#include "vga.h"

unsigned char *backBuffer;  // The back buffer we draw to
int height = 550;  // Height of the screen... I hope

// Swap the buffers by putting whatever is in backbuffer into video memory and vice-verso
void swapBuffers() {
    memcpy(framebuffer, backBuffer, height * getPitch());
}

// Clear the back buffer by setting all of its pixles to black
void clearBuffer() {
    //memcpy(backBuffer, 0, 500*(getPitch()));

    for (int i = 0; i <= 500; i++) {
        for (int j = 0; j <= 80*8; j++) {
            plotBuffer(j, i, 0);
        }
    }
}

// Plot a pixle to the backbuffer
void plotBuffer(int x, int y, unsigned int color) {
    backBuffer[getPitch() * y + 4 * x + 0] = color & 0xff;
    backBuffer[getPitch() * y + 4 * x + 1] = (color >> 8) & 0xff;
    backBuffer[getPitch() * y + 4 * x + 2] = (color >> 16) & 0xff;
}

// Plot a line to backbuffer
void lineBuffer(int x1, int y1, int x2, int y2, unsigned int color) {
    /*
    Plot a line from (x1, y1) to (x2, y2) in a certain color.
    */

    float slope;

    if (x2 - x1 != 0) { 
        slope = (y2 - y1)/(x2 - x1);  // Calculate the slope of the line 

        // Plot the line
        for (int i = x1; i < x2; i++) {
            int y = slope * (i - x1) + y1;
            plotBuffer(i, y, color);
        }
    } 
    // Be able to plot vertical lines
    else {
        for (int i = y1; i < y2; i++) { plotBuffer(x1, i, color); }
    } 

}

// Plot a rectangle to backbuffer
void drawRectangleBuffer(int x, int y, int width, int height, unsigned int c) {

    lineBuffer(x, y, x+width, y, c);
    lineBuffer(x, y+height, x+width, y+height, c);
    lineBuffer(x, y, x, y+height, c);
    lineBuffer(x+width, y, x+width, y+height, c);

    for (int i = x; i < width+x; i++) { lineBuffer(i, y, i, y+height, c); }
}

// Initialize the double buffer bt creating the backbuffer
void initDoubleBuffer() {
    //backBuffer = ((unsigned char *) (kmalloc(sizeof(framebuffer))));
    int s;
    for (int i = 0; i < framebuffer; i++) { s = i; }
    backBuffer = ((unsigned char *) (kmalloc(s)));
}