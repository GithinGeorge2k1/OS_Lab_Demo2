#include <ncurses.h>
#include "util.h"
#include <iostream>

#define READ_MODE 0;
#define WRITE_MODE 1;
// Cannot Scroll -> Needs we need a page datastructure and render the boxes dynamically ..!

// We Will Do THIS
// OR create overlapping windows and use refresh keys and Page Variable at top!!
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
        switch (trigger)
        {
        case '\n':
            MODE=WRITE_MODE;
            wbkgd(windowList[0],COLOR_PAIR(1));
            wmove(currWindow,sbHt/2,sbWd/2);
            wrefresh(currWindow);
                        
            while((a=getch())!='\n'){
                val = val*10 + (a-48);  
            }
            char * thing;
            asprintf(&thing, "%d", val);
            // wattron(currWindow,A_BOLD);
            // wattron(currWindow,COLOR_PAIR(1));
            // attron(COLOR_PAIR(1));
            print_in_middle(currWindow,sbHt/2,0,sbWd,thing);
            free(thing);
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

    // int writeText(char trigger){
    //     if(MODE){
    //         char * thing;
    //         asprintf(&thing, "%C", trigger);
    //         // wattron(currWindow,A_BOLD);
    //         // wattron(currWindow,COLOR_PAIR(1));
    //         // attron(COLOR_PAIR(1));
    //         print_in_middle(currWindow,sbHt/2,1,sbWd-2,thing);
    //         free(thing);
    //         // attroff(COLOR_PAIR(1));
    //         return 1;
    //     }
    //     return 0;
    // }
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
    // printw("%d %d",maxY,maxX);
    //Main Box
    bounds* bdMainBox = new bounds(23,39,5,5);
    // WINDOW *mainBox = createWin(bdMainBox);

    // wattron(mainBox,A_BOLD);
    // wattron(mainBox,COLOR_PAIR(0));
    // // wborder(mainBox,'*','*','*','*','*','*','*','*');   
    // box(mainBox,0,0);   
    // wattroff(mainBox,COLOR_PAIR(0));
    // wattroff(mainBox,A_BOLD);

    refresh();
    // wrefresh(mainBox);
    
    int cellsPerRow = 4,cellsPerColumn = 4;

    int rows=4,columns=4;    
    int sbHt = (bdMainBox->height)/cellsPerRow,sbWd = (bdMainBox->width)/cellsPerColumn;

    int ColorVal = 0;
    WINDOW **SubBoxes = new WINDOW*[rows*columns];

    
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
    
       
        // mRow++;
        // if(mRow>(bdMainBox->height) - 2){
        //     mRow = 1;
        // }

        // char * thing;
        // asprintf(&thing, "Key Pressed %C", c);
        // wattron(mainBox,A_BOLD);
        // wattron(mainBox,COLOR_PAIR(1));
        // // attron(COLOR_PAIR(1));
        // print_in_middle(mainBox,mRow,1,bdMainBox->width -2,thing);
        // free(thing);
        // attroff(COLOR_PAIR(1));

        // wmove(mainBox,mRow,1);
        // wprintw(mainBox,"This is %c",c);
    }

    endwin();
    return 0;


}
