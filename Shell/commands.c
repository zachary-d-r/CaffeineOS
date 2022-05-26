#include "../Kernel/system.h"
#include "shell.h"
#include "../Kernel/Drivers/vga.h"

// Tell user what the different commands do
void help() {
    print("Commands:\n");
    print("     exit - halts the cpu\n");
    print("     ping - prints pong to the screen\n");
    print("     clear - clears the screen\n");
    print("     color {int} - Change the color of the text on the screen\n");
    print("     background {int} - Change the background color and clear the screen\n");
    print("     echo {string} - Prints the string to the console");
}

// Test command to get things working
void ping() {
    print("pong");
}

// Your classic echo command
void echo(char *args) {
    print(args);
}

// Clear the screen
void clearCmd() {
    clear();
    moveCursor(0, -2);
}

// Stop the cpu
void quit() {
    __asm__  __volatile__  ("hlt");
}

// Go back to the main menu
void exit() {
    init();
}

// Change the text color of the screen
void color(char *args) {
    int color = strToInt(args);
    if (color >= 1 && color <= 15) { setColor((char)color, getBackground()); }
    else { print("Unknown color: "); print(args); print("Please pick a color from 1-15"); }
}

// Change the background color of the screen and also clear the screen
void background(char *args) {
    int color = strToInt(args);
    if (color >= 1 && color <= 15) { setColor(getForeground(), color); }
    else { print("Unknown color: "); print(args); print("Please pick a color from 1-15"); }
    clearCmd();
}