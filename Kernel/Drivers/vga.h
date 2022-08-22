#ifndef __VGA_H
#define __VGA_H

#define PIXLES_HEIGHT 480
#define PIXLES_WIDTH 640

#define CHAR_SCREEN_HEIGHT PIXLES_HEIGHT/CHAR_HEIGHT
#define CHAR_SCREEN_WIDTH PIXLES_WIDTH/CHAR_WIDTH

#define CHAR_HEIGHT 16
#define CHAR_WIDTH 8


extern void initializeVGA(unsigned long addr);
extern void plotPixle(int x, int y, unsigned int color);
extern void test();
extern void plotLine(int x1, int y1, int x2, int y2, unsigned int color);
extern void drawRectangle(int x, int y, int width, int height, unsigned int c);
extern void putchVGA(char character, int x, int y, unsigned int color);
extern void moveCursor(int x, int y);
extern void putch(char c);
extern void print(char *str);
extern void setColor(unsigned int foreground, unsigned int background);
extern void clear();
extern int getCursorX();
extern int getCursorY();
extern int getBackground();
extern int getForeground();
extern void moveCursorRealative(int x, int y);
extern void plotPixleBuffer(int x, int y, unsigned int color, unsigned char *buffer);
extern int getPitch();
extern void scroll();
extern unsigned char *framebuffer;

#endif 