#include "hirakata.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "hirakata.h"
#include "silk.h"
//int r = rand();

segment hiragana[] = {
    {"a", 0x3042},
    {"i", 0x3044},
    {"u", 0x3046},
    {"e", 0x3048},
    {"o", 0x304A},
    {"ka", 0x304B},
    {"ga", 0x304C},
    {"ki", 0x304D},
    {"gi", 0x304E},
    {"ku", 0x304F},
    {"gu", 0x3050},
    {"ke", 0x3051},
    {"ge", 0x3052},
    {"ko", 0x3053},
    {"go", 0x3054},
    {"sa", 0x3055},
    {"za", 0x3056},
    {"si", 0x3057},
    {"zi", 0x3058},
    {"su", 0x3059},
    {"zu", 0x305A},
    {"se", 0x305B},
    {"ze", 0x305C},
    {"so", 0x305D},
    {"zo", 0x305E},
    {"ta", 0x305F},
    {"da", 0x3060},
    {"chi", 0x3061},
    {"di", 0x3062},
    {"tsu", 0x3064},
    {"du", 0x3065},
    {"te", 0x3066},
    {"de", 0x3067},
    {"to", 0x3068},
    {"do", 0x3069},
    {"na", 0x306A},
    {"ni", 0x306B},
    {"nu", 0x306C},
    {"ne", 0x306D},
    {"no", 0x306E},
    {"ha", 0x306F},
    {"ba", 0x3070},
    {"pa", 0x3071},
    {"hi", 0x3072},
    {"bi", 0x3073},
    {"pi", 0x3074},
    {"hu", 0x3075},
    {"bu", 0x3076},
    {"pu", 0x3077},
    {"he", 0x3078},
    {"be", 0x3079},
    {"pe", 0x307A},
    {"ho", 0x307B},
    {"bo", 0x307C},
    {"po", 0x307D},
    {"ma", 0x307E},
    {"mi", 0x307F},
    {"mu", 0x3080},
    {"me", 0x3081},
    {"mo", 0x3082},
    {"ya", 0x3084},
    {"yu", 0x3086},
    {"yo", 0x3088},
    {"ra", 0x3089},
    {"ri", 0x308A},
    {"ru", 0x308B},
    {"re", 0x308C},
    {"ro", 0x308D},
    {"wa", 0x308F},
    {"wi", 0x3090},
    {"we", 0x3091},
    {"wo", 0x3092},
    {"n", 0x3093}
};

int top[] = {7, 11, 11, 11, 3};
int left[] = {13, 13, 23, 3, 13};

int main() {
    srand(time(NULL));

    unflagTerm(ICANON | ECHO);
    hideCursor();

    config conf;
    int ch[4];
    int i;
    long k;

    int hiragana_count = sizeof(hiragana) / sizeof(segment);

    while (1) {
        clearScreen();
        makeQuestion(ch, hiragana_count);
        conf.answer = rand() % 4;
        conf.type = rand() % 2;
        conf.red = -1;
        conf.green = -1;
        printLayout(hiragana, ch, &conf);
        do {
            k = waitKey(100);
            if (k == 'q' || k == 'Q') break;
        } while (k - KEY_UP < 0 || k - KEY_UP > 3);
        if (k == 'q' || k == 'Q') break;
        conf.red = k - KEY_UP;
        conf.green = conf.answer;
        printLayout(hiragana, ch, &conf);
        k = waitKey(100);
        if (k == 'q' || k == 'Q') break;
    }

    resetColor();
    clearScreen();
    showCursor();
    resetTerm();

    return 0;
}

void makeQuestion(int* choices, int count) {
    int i, j, f;
    for (i = 0; i < 4; i++) {
        do {
            f = 0;
            choices[i] = rand() % count;
            for (j = 0; j < i; j++)
                if (choices[j] == choices[i]) {
                    f = 1;
                    break;
                }
        } while (f);
    }
}

void printLayout(segment* table, int* choices, config* conf) {
    int i;
    if (conf->type == ASK_LETTER) {
        printTextButton(left[4], top[4], table[choices[conf->answer]].name, GREY);
        for (i = 0; i < 4; i++) {
            int color = GREY;
            if (i == conf->red) color = RED;
            if (i == conf->green) color = GREEN;
            printLetterButton(left[i], top[i], table[choices[i]].letter, color);
        }
    } else if (conf->type == ASK_NAME) {
        printLetterButton(left[4], top[4], table[choices[conf->answer]].letter, GREY);
        for (i = 0; i < 4; i++) {
            int color = GREY;
            if (i == conf->red) color = RED;
            if (i == conf->green) color = GREEN;
            printTextButton(left[i], top[i], table[choices[i]].name, color);
        }
    }
    fflush(stdout);
    resetColor();
}

void printLetterButton(int x, int y, int c, int color) {
    setBgColor(color);
    setFgColor(WHITE);
    printFrame(x, y, 1);
    printUtf8(c);
}

void printTextButton(int x, int y, char* s, int color) {
    setBgColor(color);
    setFgColor(WHITE);
    printFrame(x, y, strlen(s));
    printf("%s", s);
}

void printFrame(int x, int y, int len) {
    const int hei = 1;
    const int wid = BTN_WID;
    const int x_ = x + wid + 1;
    const int y_ = y + hei + 1;
    int i;

    // print corners
    gotoXY(x, y);
    printUtf8(BOX_DR);
    gotoXY(x_, y);
    printUtf8(BOX_DL);
    gotoXY(x, y_);
    printUtf8(BOX_UR);
    gotoXY(x_, y_);
    printUtf8(BOX_UL);

    // print lines
    for (i = x + 1; i < x_; i++) {
        gotoXY(i, y);
        printUtf8(BOX_H);
        gotoXY(i, y_);
        printUtf8(BOX_H);
    }
    for (i = y + 1; i < y_; i++) {
        gotoXY(x, i);
        printUtf8(BOX_V);
        gotoXY(x_, i);
        printUtf8(BOX_V);
    }

    // fill
    for (i = x + 1; i < x_; i++) {
        gotoXY(i, y + 1);
        printf(" ");
    }

    // align content
    gotoXY(x + (wid - len) / 2 + 1, y + 1);
}
