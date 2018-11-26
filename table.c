#include <stdio.h>
#include "table.h"
#include "silk.h"

int main(){
    clearScreen();
    hideCursor();
    unflagTerm(ICANON | ECHO);

    int i;
    for (i=0; i<5; i++){
        table(i*0x100, (i+1)*0x100, 0);
    }
        (0x1E00, 0x1F00, 0);
    table(0x2100, 0x2200, 0);
    table(0x2200, 0x2300, 0);
    table(0x2300, 0x2400, 0);
    table(0x2500, 0x2600, 0);
    table(0x2600, 0x2700, 0);
    table(0x3000, 0x3100, 0);

    resetTerm();
    showCursor();
    resetColor();
    clearScreen();
    return 0;
}

void table(int start, int end, int blko){
    int i, px, py;
    int mx = 10, my = 5;
    int blk = 0x10;

    int blkn = 1;
    for (i=0; i<blko; i++) blkn *= 2;

    int ln = blk * blkn;
    int ln_st = start - start%ln;

    for (i=ln_st; i<end; i+=ln){
        gotoXY(0, (i - ln_st)/ln + my);
        printf("%6x", i);
    }

    for (i=start; i<end; i++){
        px = (i%ln)/blk*2 + mx;
        py = my;
        gotoXY((i - start)%ln*2 + px, (i - start)/ln + py);
        printUtf8(i);
    }
    getchar();

}
