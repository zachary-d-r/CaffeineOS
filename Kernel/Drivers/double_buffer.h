#ifndef __DOUBLE_BUFFER_H
#define __DOUBLE_BUFFER_H

extern void initDoubleBuffer();
extern void drawRectangleBuffer(int x, int y, int width, int height, unsigned int c);
extern void lineBuffer(int x1, int y1, int x2, int y2, unsigned int color);
extern void plotBuffer(int x, int y, unsigned int color);
extern void clearBuffer();
extern void swapBuffers();

#endif