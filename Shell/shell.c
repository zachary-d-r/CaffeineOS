#include "../Kernel/system.h"
#include "shell.h"
#include "../Drivers/vga.h"

int commandCursor;
char commandBuffer[256];
char *empty;

// Start the shell
void loadShell() {
    changeProcess("shell");
    clear();
    
    moveCursor(0, 0);
    print(" _____     ___ ___ _             _____ _____    _____ _       _ _ \n");
    print("|     |___|  _|  _|_|___ ___ ___|     |   __|  |   __| |_ ___| | |\n");
    print("|   --| .'|  _|  _| | -_|   | -_|  |  |__   |  |__   |   | -_| | |\n");
    print("|_____|__,|_| |_| |_|___|_|_|___|_____|_____|  |_____|_|_|___|_|_|\n");
    print("                                                                  \n>");

}

// Get the command from the buffer
char *getCommand(char *buffer) {
    
    char *cmd;
    
    memset(cmd, 0, strlen(cmd));  // Clear cmd

    int i;
    while(buffer[i] != ' ') {
        append(cmd, buffer[i]);
        i++;
    }

    return cmd;
}

// Get the arguments for a command
char *getArgs(char *buffer) {
    int cmdLen = strlen(getCommand(buffer))+1;  // Get the length of the command name
    char *args;  // String that will hold our arguments

    memset(args, 0, strlen(args));  // Clear args

    for (int i = 0; i < strlen(buffer)-cmdLen; i++) { args[i] = buffer[i+cmdLen]; }  // Add each character to args
    
    return args;  // Return the args
}

// Handle what happens in the shell after a key has been pressed
void shellHandler(char c) {
    if (c == '\n') { append(commandBuffer, ' '); executeCommand(commandBuffer); }
    else if (c == 0x08) { remove(commandBuffer); if (getCursorX() > 1) { putch(0x08); } }
    else { putch(c); append(commandBuffer, c); }
}

// Execute a command when enter is pressed
void executeCommand(char *buffer) {
    
    putch('\n');  // Print a newline to the screen
    int enter = 0;

    char *cmd = getCommand(buffer);

    if (strcmp(cmd, "ping") == 0) { ping(); }
    else if (strcmp(cmd, "echo") == 0) { echo(getArgs(buffer)); }
    else if (strcmp(cmd, "clear") == 0 || strcmp(cmd, "cls") == 0) { clearCmd(); }
    else if (strcmp(cmd, "help") == 0) { help(); }
    else if (strcmp(cmd, "quit") == 0) { quit(); }
    else if (strcmp(cmd, "color") == 0) { color(getArgs(buffer)); }
    else if (strcmp(cmd, "background") == 0) { background(getArgs(buffer)); }

    else if (strcmp(cmd, "") == 0) { enter = 1; }
    else { print("Command "); print(cmd); print(" does not exist. Please try again..."); }  // Print -1 if the command does not exist

    if (enter != 1) { print("\n\n>"); }
    else { print(">"); }

    if (strcmp(cmd, "exit") == 0) { exit(); }

    commandBuffer[0] = '\0';
}