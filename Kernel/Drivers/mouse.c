#include "../system.h"

/*
Byte Structure
--------------
Byte 1:  
    Y overflow,	X overflow,	Y sign bit,	X sign bit,	Always 1, Middle Btn, Right Btn, Left Btn

Byte 2:
    X position

Byte 3:
    Y position
*/

unsigned char mouseCycle = 0;
unsigned char mouseByte[3];
unsigned char mouseX = 0;  // The mouse's X position
unsigned char mouseY = 0;  // The mouse's Y position

// Handle the mouse input by reading the bytes from port 0x60
void mouseHandler(struct regs *r) {
    switch (mouseCycle) {
    case 0:
        mouseByte[0] = inportb(0x60);  // Set the first byte to what we read from port 0x60
        mouseCycle++;

        if ((mouseByte[0] >> 6) & 1) { print("Mouse"); }

        break;
    case 1:
        mouseByte[1] = inportb(0x60);  // Set the second byte to be what we read from port 0x60 - the x position
        mouseCycle++;
        break;
    case 2:
        mouseByte[2] = inportb(0x60);  // Set the third byte to be what we read from port 0x60 - the y position
        
        mouseX = mouseByte[1];  // Set the x position
        mouseY = mouseByte[2];  // Set the y position
        mouseCycle = 0;  // Restart getting input from the mouse
        break;
    
    default:
        break;
    }
    printInt((mouseByte[0] >> 6) & 1);
}

// A delay for every time it can read from the mouse or write to the mouse
inline void mouseWait(unsigned char type) {
    unsigned int timeOut = 100000;  // The delay

    if (type == 0) {
        while (timeOut--) { if ((inportb(0x60) & 1) == 1) { return; } }  // Wait until we see input from the mouse
        return;
    }
    else { while (timeOut--) {if ((inportb(0x60) & 2) == 0) { return; } } return; }  // Wait until we see a signal from the mouse
}

// Write to the mouse
void writeMouse(unsigned char data) {
    mouseWait(1);  // Wait to be able to send a command
    outportb(0x60, 0xD4);  // Tell the mouse we want to write to it
    mouseWait(1);  // Wait for it it allow us to write to it
    outportb(0x60, data);  // Write the data to the mouse
}

// Get a response from the mouse
unsigned char readMouse() {
    mouseWait(1);  // Wait to be able to read the mouse
    return inportb(0x60);  // Read bytes from 0x60
}

// Install the mouse
void installMouse() {
    unsigned char status;

    // Enable auxilury mode
    mouseWait(1);
    outportb(0x64, 0xA8);

    // Enable the interrupts
    mouseWait(1);
    outportb(0x64, 0xA8);
    mouseWait(0);
    status = inportb(0x60 | 2);
    mouseWait(0);
    outportb(0x64, 0x60);
    mouseWait(1);
    outportb(0x60, status);

    // Tell the mouse to use default settings
    writeMouse(0xF6);
    readMouse();

    // Enable the mouse
    writeMouse(0xF4);
    readMouse();

    // Set up the mouse irq handles
    installIrqHandler(12, mouseHandler);

    
}