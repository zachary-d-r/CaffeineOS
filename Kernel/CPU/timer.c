#include "../system.h"
#include "../../Applications/applications.h"
#include "../Drivers/double_buffer.h"

// Set channel 0's data register on the PIT
void timerPhase(int hz) {
    int divisor = 1193180 / hz;  // Calculate our divisor
    outportb(0x43, 0x36);  // Set our command byte 0x36 
    outportb(0x40, divisor & 0xFF);  // Set low byte of divisor
    outportb(0x40, divisor >> 8);  // Set high byte of divisor
}

volatile unsigned int timerTicks = 0;

// Handle the timer by incrementing timerTicks every wime the timer fires. By default, the timer fires 18.222 times a second
void timerHandler(struct regs *r) {
    timerTicks++;  // Increment our tick count
    //swapBuffers();


    // Every 18 cycles (about 1 second) we display a message on the screen
    //if (timerTicks % 2 == 0 && gameRunning == 1) { mainLoop(); }
}

// Wait for a specified amount of ticks
void waitHz(int ticks) {
    unsigned long eticks;

    eticks = timerTicks + ticks;
    while(timerTicks < eticks) { __asm__ __volatile__ ("sti//hlt//cli"); };
}

void setLoop(int ticks, void *func) {
    waitHz(ticks);
    //func();
}

// Wait for a specified amount of miliseconds
void wait(int ms) { waitHz(ms*0.018222); }

void installTimer() { installIrqHandler(0, timerHandler); timerPhase(50); }  // Sets up the system clock by installing the time handler to IRQ0
