#include "silk.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/select.h>

// terminal manipulation
int init = 0;
struct termios bk;

void savebk() {
    if (!init) {
        tcgetattr(STDIN_FILENO, &bk);
        init = 1;
    }
}

void unflagTerm(tcflag_t flag) {
    savebk();
    static struct termios newt;
    tcgetattr(STDIN_FILENO, &newt);
    newt.c_lflag &= ~(flag);
    newt.c_cc[VMIN] = 1;
    newt.c_cc[VTIME] = 1;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
}

void resetTerm() {
    tcsetattr(STDIN_FILENO, TCSANOW, &bk);
}

long readKey(int delay) {
    long res = 0;
    int i, h;
    char c;
    struct timeval tv;

    tv.tv_sec = 0;
    tv.tv_usec = delay * 1000;
    select(STDIN_FILENO, NULL, NULL, NULL, &tv);
    ioctl(STDIN_FILENO, FIONREAD, &h);
    for (i=0; i<h; i++) {
        read(STDIN_FILENO, &c, 1);
        res <<= 8;
        res += c;
    }
    return res;
}

long waitKey(int delay){
    long res;
    do {
        res=readKey(delay);
    } while (!res);
    return res;
}

// escape sequence sender
void setFgColor(int color) {
    printf("\033[38;5;%dm", color);
}

void setBgColor(int color) {
    printf("\033[48;5;%dm", color);
}

void resetColor() {
    printf("\033[0m");
}

void clearScreen() {
    printf("\033[2J\033[3J\033[H");
}

void gotoXY(int x, int y) {
    printf("\033[%d;%df", y, x);
}

void hideCursor(int x, int y) {
    printf("\033[?25l");
}

void showCursor(int x, int y) {
    printf("\033[?25h");
}

// unicode helper
void printUtf8(int c) {
    int show = 0;

    // Allow blocks:
    // Basic Latin (filtered)
    show |= c > 0x1F && c < 0x7F;
    // Latin-1 Supplement (filtered)
    show |= c > 0xA0 && c < 0x100;
    // Latin Extended-A
    // Latin Extended-B
    // IPA Extensions
    // Spacing Modifier Letters
    show |= c > 0xFF && c < 0x300;
    // Greek and Coptic
    show |= c > 0x370 && c < 0x400;
    // Cyrillic (filtered)
    show |= c > 0x3FF && c < 0x482;
    show |= c > 0x489 && c < 0x500;
    // Latin Extended Additional
    show |= c > 0x1DFF && c < 0x1F00;
    // Letterlike Symbols
    // Number Forms
    // Arrows
    // Mathematical Operators
    // Miscellaneous technical
    show |= c > 0x20FF && c < 0x2400;
    // Box Drawing
    // Block Elements
    // Geometric Shapes
    // Miscellaneous symbols
    show |= c > 0x24FF && c < 0x2700;
    // CJK Symbols and Punctuation
    show |= c > 0x3000 && c < 0x302A;
    show |= c > 0x302F && c < 0x303E;
    // Hiragana
    show |= c > 0x3040 && c < 0x3097;
    // Katakana
    show |= c > 0x309F && c < 0x3100;

    if (show) {
        printUtf8Raw(c);
    } else {
        printUtf8Raw(0xFFFD);
    }
}

void printUtf8Raw(int c) {
    char s[5] = "?";
    if (c > 0x1f && c < 0x80) {
        s[0] = (char)c;
        s[1] = '\0';
    } else if (c < 0x800) {
        s[0] = 0xC0 + c / 0x40;
        s[1] = 0x80 + c % 0x40;
        s[2] = '\0';
    } else if (c < 0x10000) {
        s[0] = 0xE0 + c / 0x1000;
        s[1] = 0x80 + c / 0x40 % 0x40;
        s[2] = 0x80 + c % 0x40;
        s[3] = '\0';
    } else if (c < 0x110000) {
        s[0] = 0xF0 + c / 0x40000;
        s[1] = 0x80 + c / 0x1000 % 0x40;
        s[2] = 0x80 + c / 0x40 % 0x40;
        s[3] = 0x80 + c % 0x40;
        s[4] = '\0';
    }
    printf("%s", s);
}
