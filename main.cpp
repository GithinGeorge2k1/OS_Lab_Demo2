#include <ncurses.h>
#include "util.h"
#include <iostream>

using namespace std;


int moveCursor(WINDOW *win,char trigger){
    int x,y;
    int yWin,xWin;
    getyx(win,x,y);
    printw("%d %d",x,y);
    getmaxyx(win,yWin,xWin);
    int begy = getbegy(win);
    switch (trigger)
    {
    case 'j':
        y++;
        if(yWin-2<y){
            y=begy;
        }
        wmove(win,y,x);
        return 0;
        break;
    
    default:
        break;
    }
    wrefresh(win);
    return -1;
}

int main(){

    initscr();

    cbreak();
    noecho();
    
    if(!has_colors()){
        printw("Does Not Have Color!! Enjoy");
    }
    start_color();	
    // init_pair(0,COLOR_MAGENTA,COLOR_BLACK);
    init_pair(1,COLOR_MAGENTA,COLOR_BLACK);
    init_pair(2, COLOR_YELLOW, COLOR_BLACK);
    init_pair(3, COLOR_GREEN, COLOR_BLACK);
    init_pair(4, COLOR_CYAN, COLOR_BLACK);
    init_pair(5, COLOR_BLUE, COLOR_BLACK);
    init_pair(6, COLOR_RED, COLOR_BLACK);


    init_pair(7, COLOR_BLACK, COLOR_WHITE);
    init_pair(8, COLOR_BLUE, COLOR_BLACK);
    init_pair(9, COLOR_RED, COLOR_BLACK);

    

    int maxX,maxY;
    getmaxyx(stdscr, maxY, maxX);
    // printw("%d %d",maxY,maxX);
    //Main Box
    bounds* bdMainBox = new bounds(22,38 + 64,5,5);
    WINDOW *mainBox = createWin(bdMainBox);

    wattron(mainBox,COLOR_PAIR(0));
    // wborder(mainBox,'*','*','*','*','*','*','*','*');   
    box(mainBox,0,0);   
    wattroff(mainBox,COLOR_PAIR(0));

    refresh();
    wrefresh(mainBox);
    // keypad(mainBox,true);

    int rows=4,columns=4;    

    int sbHt = (bdMainBox->height-2)/rows,sbWd = (bdMainBox->width -2)/columns;

    int ColorVal = 0;
    WINDOW **SubBoxes = new WINDOW*[rows*columns];

    for(int i=0;i<rows;i++){
        ColorVal = i%6  + 1;
        for(int j=0;j<columns;j++){
            int rs = (bdMainBox->rowStart + 1) + i*sbHt;

            int cs = (bdMainBox->colStart +1) + j*sbWd;
            bounds *currBound = new bounds(sbHt,sbWd,rs,cs);
            SubBoxes[i*rows + j] = createWin(currBound);
            wattron(SubBoxes[i*rows + j],COLOR_PAIR(ColorVal));
            // wborder(SubBoxes[i*rows + j],'*','*','*','*','*','*','*','*'); 
            box(SubBoxes[i*rows + j],0,0); 
            wattroff(SubBoxes[i*rows + j],COLOR_PAIR(ColorVal));
            refresh();
            wrefresh(SubBoxes[i*rows + j]);
            delete currBound;
        }
        
    }
    // bounds* bdBox1 = new bounds((bdMainBox->height - 2)/4,(bdMainBox->width - 2)/4,
    //  bdMainBox->rowStart + 1,bdMainBox->colStart +1); 
    // WINDOW *box1 = createWin(bdBox1);
    // // box(box1,'*','*');

    // wborder(box1,'*','*','*','*','*','*','*','*');
    // refresh();
    // wrefresh(box1);
    // keypad(box1,true);

    int mRow=0;
    char c;

    while((c=wgetch(mainBox)) != 'q'){
        if(moveCursor(mainBox,c)==-1){    
            mRow++;
            if(mRow>(bdMainBox->height) - 2){
                mRow = 1;
            }

            char * thing;
            asprintf(&thing, "Key Pressed %C", c);
            wattron(mainBox,COLOR_PAIR(1));
            // attron(COLOR_PAIR(1));
            print_in_middle(mainBox,mRow,1,bdMainBox->width -2,thing);
            free(thing);
            // attroff(COLOR_PAIR(1));



            // wmove(mainBox,mRow,1);
            // wprintw(mainBox,"This is %c",c);
        }
    }

    // while((c=wgetch(box1)) != 'q'){
    //     mRow++;
    //     if(mRow>(bdBox1->height) - 2){
    //         mRow = 1;
    //     }
    //     wmove(box1,mRow,1);
    //     wprintw(box1,"This is %c",c);
    // }

    endwin();
    return 0;


}