#ifndef HIRAKATA_H
#define HIRAKATA_H

#define BOX_H 0x2500
#define BOX_V 0x2502

#define BOX_DR 0x250C
#define BOX_DL 0x2510
#define BOX_UR 0x2514
#define BOX_UL 0x2518

#define BTN_WID 7

#define BLACK 0
#define GREY 8
#define RED 9
#define GREEN 2
#define WHITE 15

#define KEY_REF 0x1b5b40
#define KEY_UP 0x1b5b41
#define KEY_DOWN 0x1b5b42
#define KEY_RIGHT 0x1b5b43
#define KEY_LEFT 0x1b5b44

enum {
    ASK_LETTER,
    ASK_NAME
};

typedef struct {
    char* name;
    int letter;
} segment;

typedef struct {
    int type;
    int answer;
    int red;
    int green;
} config;


void printLayout(segment* table, int* choices, config* conf);
void makeQuestion(int* choices, int count);
void printTextButton(int x, int y, char* s, int color);
void printLetterButton(int x, int y, int c, int color);
void printFrame(int x, int y, int len);

#endif
