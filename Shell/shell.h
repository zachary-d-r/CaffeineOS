#ifndef __SHELL_H
#define __SHELL_H

// shell.c
extern void loadShell();
extern void shellHandler(char c);

// commands.c
extern void ping();
extern void echo(char *buffer);
extern void clearCmd();
extern void help();
extern void exit();
extern void color(char *args);
extern void background(char *args);
extern void quit();

#endif