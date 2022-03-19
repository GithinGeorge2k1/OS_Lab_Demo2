#include <ncurses.h>
#include "util.h"
#include <iostream>

#define READ_MODE 0;
#define WRITE_MODE 1;
int vals[16] = {-1};

using namespace std;

struct Page{
    int startRow,startCol;
    int id;
};

struct Control{
    int posY,posX;
    WINDOW **windowList;
    WINDOW *currWindow;
    int MODE;
    int Row,Col;
    int currentIndex;
    int sbHt, sbWd;
    Control(WINDOW **wl,int r, int c,int ht,int wd) : posY(0), posX(0),windowList(wl),currWindow(wl[0]),MODE(0),Row(r),Col(c),currentIndex(0),
    sbHt(ht), sbWd(wd){
        moveUtility();
        wbkgd(windowList[0],COLOR_PAIR(2));
        wrefresh(windowList[0]);
        for(int i=0; i<16; i++){
            vals[i]=-1;
        }   
    }
    

    int moveCursor(char trigger){
       
        switch (trigger)
        {
        case 'w':
            currentIndex=currentIndex-Col;

            if(currentIndex<0){
                currentIndex +=Col;
            }
            break;
        case 'a':
            currentIndex--;
            if(currentIndex<0){
                currentIndex++;
            }
            break;
        case 's':
            
            currentIndex+=Col;

            if(currentIndex>= (Row*Col)){
                currentIndex -=Col;
            }
            break;
        case 'd':
            
            currentIndex++;
            if(currentIndex>= (Row*Col)){
                currentIndex--;
            }
            break;
        default:
            return 0;
            break;
        }
        moveUtility();
        return 1;
    }

    int changeMode(char trigger){
        char a;
        int val = 0;
        bool overwritten = false;

        switch (trigger)
        {
        case '\n':
            MODE=WRITE_MODE;
            // wbkgd(windowList[0],COLOR_PAIR(1));
            move(22,0);
            printw("Press Enter Key To exit edit mode");
            refresh();
            print_in_middle(currWindow,(sbHt-2)/2,0,sbWd,"      ");
            wmove(currWindow,(sbHt-2)/2,(sbWd-2)/2);
            wrefresh(currWindow);

            char * thing;    
            val=0;       
            while((a=getch())!='\n'){
                if(a<'0' || a>'9'){
                    continue;
                }
                overwritten = true;
                int temp = val*10 + (a-48);
                if(temp>=999){
                    continue;
                }  
                val = temp;
                asprintf(&thing, "%d", val);
                print_in_middle(currWindow,(sbHt-2)/2,0,sbWd,thing);
            }
            wmove(stdscr,22,0);
            wprintw(stdscr,"Are u sure to update? Enter 'y' if yes or 'n' otherwise");
            if(getch()=='y'){
                if(overwritten){
                    vals[currentIndex] = val;
                }
            }else{
                wmove(currWindow,(sbHt-2)/2,(sbWd-2)/2);
                wrefresh(currWindow);
                if(vals[currentIndex]!= -1){
                    val = vals[currentIndex];
                    asprintf(&thing, "%d", val);
                    print_in_middle(currWindow,(sbHt-2)/2,0,sbWd,"      ");
                    print_in_middle(currWindow,(sbHt-2)/2,0,sbWd,thing);
                }else{
                    
                    print_in_middle(currWindow,(sbHt-2)/2,0,sbWd,"      ");
                }
                wmove(currWindow,(sbHt-2)/2,(sbWd-2)/2);
                wrefresh(currWindow);
            }
            // clear();
            if(overwritten)
                free(thing);
            wmove(stdscr,22,0);
            clrtoeol();
            wrefresh(stdscr);
            wmove(currWindow,0,0);
            wbkgd(windowList[0],COLOR_PAIR(2));
            wrefresh(currWindow);
            return 1;
            break;
        default:
            return 0;
            break;
        }
        moveUtility();
        return 0;
    }

private:

    void moveUtility(){
        wbkgd(currWindow,COLOR_PAIR(7));
        currWindow = windowList[currentIndex];
        wmove(currWindow,0,0);
        wbkgd(currWindow,COLOR_PAIR(2));
        refresh();
        wrefresh(currWindow);
    }

};

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
    init_pair(7, COLOR_WHITE, COLOR_BLACK);
    init_pair(8, COLOR_BLACK, COLOR_BLUE);
    init_pair(9, COLOR_BLACK, COLOR_RED);
    init_pair(10, COLOR_BLACK, COLOR_WHITE);
    

    int maxX,maxY;
    getmaxyx(stdscr, maxY, maxX);

    //Main Box
    bounds* bdMainBox = new bounds(19,35,6,10);

    refresh();
    
    wmove(stdscr,0,0);
    wprintw(stdscr,"Instructions:\nPress WASD keys to move in between the cells\nPress ENTER to edit a cell\nPress Q to quit the program");
    
    int cellsPerRow = 4,cellsPerColumn = 4;
    int rows=4,columns=4;    
    int sbHt = (bdMainBox->height)/cellsPerRow,sbWd = (bdMainBox->width)/cellsPerColumn;
    int ColorVal = 0;
    WINDOW **SubBoxes = new WINDOW*[rows*columns];

    
    wattron(stdscr,COLOR_PAIR(1));
    int k =1; 
    for(int i=7; i<20; i=i+rows){
        wmove(stdscr,i,0);
        wprintw(stdscr,"Stduent %d",k);
        k++;
        refresh();
        wrefresh(stdscr);
    }
    wattroff(stdscr,COLOR_PAIR(1));
    
    k=1;
    wattron(stdscr,COLOR_PAIR(1));
    for(int i=11; i<37; i=i+8){
        wmove(stdscr,5,i);
        wprintw(stdscr,"Prof %d",k);
        k++;
        refresh();
        wrefresh(stdscr);
    }
    wattroff(stdscr,COLOR_PAIR(1));
    
    for(int i=0;i<rows;i++){
        ColorVal = i%6  + 1;
        for(int j=0;j<columns;j++){
            int rs = (bdMainBox->rowStart) + (i%cellsPerRow)*sbHt ;

            int cs = (bdMainBox->colStart) + (j%cellsPerColumn)*sbWd ;

            bounds *currBound = new bounds(sbHt,sbWd,rs,cs);
            SubBoxes[i*rows + j] = createWin(currBound);
            wattron(SubBoxes[i*rows + j],A_BOLD);
            wattron(SubBoxes[i*rows + j],COLOR_PAIR(ColorVal));
            // wborder(SubBoxes[i*rows + j],'`','`','`','`','`','`','`','`'); 
            box(SubBoxes[i*rows + j],0,0); 
            wattroff(SubBoxes[i*rows + j],COLOR_PAIR(ColorVal));
            wattroff(SubBoxes[i*rows + j],A_BOLD);
            refresh();
            wrefresh(SubBoxes[i*rows + j]);
            delete currBound;
        }       
    }
    
    
    int mRow=0;
    char c;

    Control *controlPtr = new Control(SubBoxes,rows,columns,sbHt,sbWd);
    while((c=getch()) != 'q'){
        if(!controlPtr->moveCursor(c)){
            controlPtr->changeMode(c);
            // if(){
            //     //  controlPtr->writeText(c);
            // }
        }
    }
    endwin();
    return 0;
}