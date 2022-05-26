#include "../system.h"
#include "../../Shell/shell.h"
#include "../../Applications/applications.h"
#include "vga.h"

char getInput(char c, int shift) {
    char r;

    if (shift == 1 & c >= 97 & c <= 122) { r = c-32; }  // Print capital letters
    
    // Shift for numbers
    else if (shift == 1 & c == 48) { r = c-7; }
    else if (shift == 1 & c == 49) { r = c-16; }
    else if (shift == 1 & c == 50) { r = c+14; }
    else if (shift == 1 & c == 51) { r = c-16; }
    else if (shift == 1 & c == 52) { r = c-16; }
    else if (shift == 1 & c == 53) { r = c-16; }
    else if (shift == 1 & c == 54) { r = c+40; }
    else if (shift == 1 & c == 55) { r = c-17; }
    else if (shift == 1 & c == 56) { r = c-14; }
    else if (shift == 1 & c == 57) { r = c-17; }

    // Shift for everything else
    else if (shift == 1 & c == '`') { r = '~'; }
    else if (shift == 1 & c == '-') { r = '_'; }
    else if (shift == 1 & c == '=') { r = '+'; }
    else if (shift == 1 & c == '`') { r = '~'; }
    else if (shift == 1 & c == '[') { r = '{'; }
    else if (shift == 1 & c == ']') { r = '}'; }
    else if (shift == 1 & c == '\\') {r =  '|'; }
    else if (shift == 1 & c == ';') { r = ':'; }
    else if (shift == 1 & c == '\'') {r =  '\"'; }
    else if (shift == 1 & c == ',') { r = '<'; }
    else if (shift == 1 & c == '.') { r = '>'; }
    else if (shift == 1 & c == '/') { r = '?'; }

    else { r = c; }

    return r;
}

// KBDUS means US Keyboard Layout, the layout for standard US keyboards
unsigned char kbdus[128] =
{
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	/* 9 */
  '9', '0', '-', '=', '\b',	/* Backspace */
  '\t',			/* Tab */
  'q', 'w', 'e', 'r',	/* 19 */
  't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',	/* Enter key */
    0,			/* 29   - Control */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* 39 */
 '\'', '`',   0,		/* Left shift */
 '\\', 'z', 'x', 'c', 'v', 'b', 'n',			/* 49 */
  'm', ',', '.', '/',   0,				/* Right shift */
  '*',
    0,	/* Alt */
  ' ',	/* Space bar */
    0,	/* Caps lock */
    0,	/* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,	/* < ... F10 */
    0,	/* 69 - Num lock*/
    0,	/* Scroll Lock */
    0,	/* Home key */
    0,	/* Up Arrow */
    0,	/* Page Up */
  '-',
    0,	/* Left Arrow */
    0,
    0,	/* Right Arrow */
  '+',
    0,	/* 79 - End key*/
    0,	/* Down Arrow */
    0,	/* Page Down */
    0,	/* Insert Key */
    0,	/* Delete Key */
    0,   0,   0,
    0,	/* F11 Key */
    0,	/* F12 Key */
    0,	/* All other keys are undefined */
};

// Converting scancode to ascii: mychar = kbdus[scancode];
// The keyboard controller has 2 main registers: a Data register at 0x60, and a Control register at 0x64.
// Anything that the keyboard wants to send the computer is stored into the Data register. 
//The keyboard will raise IRQ1 whenever it has data for us to read.

int shiftPressed = 0;  // To check if shift is pressed

// Handles the keyboard interrupt
void keyboardHandler(struct regs *r) {
    unsigned char scancode;  // The scancode from our keyboard
    scancode = inportb(0x60);  // Read from the keyboards data register which is at 0x60

    // If the top bit of the byte we read from the keyboard is set, then a key has just been released
    if (scancode & 0x80) {
        // Use this to see if the user released the shift, alt or control keys
        switch (scancode) {
          case 0xaa: shiftPressed = 0; break;
        }
    }

    // Here a key was just pressed. Holding down a key means you will get repeated key press interrupts
    else {

        /* Just to show you how this works, we simply translate
        *  the keyboard scancode into an ASCII value, and then
        *  display it to the screen. You can get creative and
        *  use some flags to see if a shift is pressed and use a
        *  different layout, or you can add another 128 entries
        *  to the above layout to correspond to 'shift' being
        *  held. If shift is held using the larger lookup table,
        *  you would add 128 to the scancode when you look for it */

        switch (scancode) {
          // Arrowkeys
          // case 0x4b: moveCursorRealative(-1, 0); scroll();  break;
          // case 0x4d: moveCursorRealative(1, 0);  scroll(); break;
          case 0x48: keyboardControll(255);  break;
          case 0x50: keyboardControll(254);  break;

          case 0x2a: shiftPressed = 1; break;  // Check if left shift is pressed
          case 0x36: shiftPressed = 1; break;  // Check if right shift is pressed

          default: { keyboardControll(getInput(kbdus[scancode], shiftPressed)); break; }
        }

    }
}

void installKeyboard() {
    installIrqHandler(1, keyboardHandler);
}