#ifndef SILK_H
#define SILK_H

// terminal manipulation
#include <termios.h>

void unflagTerm(tcflag_t flag);
void resetTerm();
long readKey(int delay);
long waitKey(int delay);

// escape sequence sending
void setFgColor(int color);
void setBgColor(int color);
void resetColor();

void clearScreen();
void gotoXY(int x, int y);

void hideCursor();
void showCursor();

// unicode helper
void printUtf8(int c);
void printUtf8Raw(int c);

#endif