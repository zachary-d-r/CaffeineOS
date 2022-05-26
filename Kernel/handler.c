#include "system.h"
#include "../Applications/applications.h"
#include "Drivers/vga.h"

char *currentProcess;

// Check the status of a process
int getStatus(char *name) {
    /*
    This function checks the status of a process like the shell

    Parameters:
    ----------
    name : char*
        The name of the process you would like to check on

    Returns:
    --------
        A 1 if the process is running and a 0 if the proccess is not
    */

    if (strcmp(name, currentProcess) == 0) { return 1; }

    else { return 0; }
}

// Change the current proccess running
void changeProcess(char *name) { currentProcess = name; /*memcpy(memProcess, name, len(name))*/ }

void keyboardControll(char key) {

    if (getStatus("main") == 1) { mainHandler(key); }
    else if (getStatus("shell") == 1) { shellHandler(key); }
    else if (getStatus("pong") == 1)  { pongHandler(key); }

}

// Start the OS
void init() {
    /*
    The purpose of this funtion is to start the OS and provide the user with a GUI
    */
   changeProcess("main");

   clear();
   moveCursor((80*8/2)-9*8, (24*16/2)-3*16);

   print("CaffeineOS");
   moveCursorRealative(-15*8, 1*16);
   print("By: Zachary Robinson");
   moveCursorRealative(-24*8, 2*16);
   print("Press 1. to launch the shell");
   moveCursorRealative(-28*8, 1*16);
   print("Press 2. to launce Pong");
   moveCursor(-1, 0);

}

// main kb handler
void mainHandler(char key) {
    if (key == '1') { loadShell(); }
    else if (key == '2') { changeProcess("pong"); outportb(0x20, 0x20); loadPong(); }
}
