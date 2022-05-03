#include <ncurses.h>
#include "util.h"
#include <iostream>
#include <fstream>

using namespace std;
#define sbHt 5
#define sbWd 15
#define cellsInRow 3
#define cellsInCol 3
#define READ_MODE 0;
#define WRITE_MODE 1;
int vals[16] = {-1};
ofstream myfile;

using namespace std;

struct Page{
    int startRow,startCol;
    int id;
};

struct Control{
    pair<int,int> top_left;
    pair<int,int> bottom_right;
    int posY,posX;
    WINDOW **windowList;
    WINDOW *currWindow;
    int arr2d[100][100];
    int MODE;
    // Total No of Rows and Columns..! 
    int Row,Col;

    // Row and Col in 3x3 matrix
    int currentRowIndex, currentColIndex;

    // Row and Col in actual Matrix
    int currRow,currCol;

    char tableType;
    int dummy;
    Control(WINDOW **wl,int r, int c,int ht,int wd,char tt) : posY(0), posX(0),windowList(wl),currWindow(wl[0]),
     MODE(0),Row(r),Col(c),currentRowIndex(0),currentColIndex(0), tableType(tt){
        moveUtility();
        // wbkgd(windowList[0],COLOR_PAIR(2));
        wrefresh(windowList[0]);
        dummy = 0;
        top_left.first = 0;
        top_left.second = 0;
        currRow = 0;
        currCol = 0;
        if(tt == 'a'){    
            bottom_right.first = 2;
            bottom_right.second = 2;
        }
        else if(tt == 's' || tt == 'l'){
            myfile << "Table Type is l" << endl;
            bottom_right.first = 0;
            bottom_right.second = 2;
        }
        else if(tt == 'f'){
            bottom_right.first = 2;
            bottom_right.second = 0;
            
        }
        for(int i=0;i<100;i++){
            for(int j=0;j<100;j++){
                arr2d[i][j] = i*100 + j;
                // myfile << arr2d[i][j] << endl;
            }
        }

        refreshWindowSet();
        moveUtility();
        dummy = 1;
        
    }
    

    int moveCursor(char trigger){
       
        switch (trigger)
        {
        case 'w':
            // currentRowIndex=currentIndex-Col;

            // if(currentIndex<0){
            //     currentIndex +=Col;
            // }
            // In Between
            if(currRow > top_left.first && currRow <=bottom_right.first)
            {
                // Nothing is going to change ..! Jst Refresh the entire setup.. with new tl and br
                currRow -= 1;
                currentRowIndex -= 1;
            }
            else if(currRow == top_left.first){
                // See if tl goes to negative
                if(top_left.first!=0){
                    top_left.first -= 1;
                    bottom_right.first -= 1;
                    currRow -= 1;
                    // refresh9windows
                }
            }
            // refreshWindowSet();
            break;
        case 'a':
            // currentIndex--;
            // if(currentIndex<0){
            //     currentIndex++;
            // }
            // In Between
            if(currCol > top_left.second && currCol <=bottom_right.second)
            {
                // Nothing is going to change ..! Jst Refresh the entire setup.. with new tl and br
                currCol -= 1;
                currentColIndex -=1;
            }
            else if(currCol == top_left.second){
                // See if tl goes to negative
                if(top_left.second!=0){
                    top_left.second -= 1;
                    bottom_right.second -= 1;
                    currCol -= 1;
                    // refresh9windows
                }
            }
            // refreshWindowSet();
            break;
        case 's':
            
            // currentIndex+=Col;

            // if(currentIndex>= (Row*Col)){
            //     currentIndex -=Col;
            // }
            // In Between
            if(currRow >= top_left.first && currRow <bottom_right.first)
            {
                // Nothing is going to change ..! Jst Refresh the entire setup.. with new tl and br
                currRow += 1;
                currentRowIndex += 1;
            }
            else if(currRow == bottom_right.first){
                // See if tl goes to negative
                if(bottom_right.first!=Row-1){
                    top_left.first += 1;
                    bottom_right.first += 1;
                    currRow += 1;
                    // refresh9windows
                }
            }
            // refreshWindowSet();
            break;
        case 'd':
            
            // currentIndex++;
            // if(currentIndex>= (Row*Col)){
            //     currentIndex--;
            // }
            // In Between
            if(currCol >= top_left.second && currCol <bottom_right.second)
            {
                // Nothing is going to change ..! Jst Refresh the entire setup.. with new tl and br
                currCol += 1;
                currentColIndex++;
            }
            else if(currCol == bottom_right.second){
                // See if tl goes to negative
                if(bottom_right.second!=Col-1){
                    bottom_right.second += 1;
                    top_left.second += 1;
                    currCol += 1;
                    // currentIndex++;
                    // refresh9windows
                }
            }
            // refreshWindowSet();
            break;
        default:
            return 0;
            break;
        }
        myfile << "Key Pressed : " << trigger << "\n\t TopLeft is : (" << top_left.first << " , " << top_left.second << ")" << endl; 
        myfile << "\tBottomRight is : (" << bottom_right.first << " , " << bottom_right.second << ")" << endl; 
        // currWindow = windowList[currentIndex];
        refreshWindowSet();
        if(dummy == 1){
            moveUtility();
        }
        
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
            move(0,0);
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
            wmove(stdscr,0,0);
            wprintw(stdscr,"Are u sure to update? Enter 'y' if yes or 'n' otherwise");
            if(getch()=='y'){
                if(overwritten){
                    vals[currentRowIndex*3 + currentColIndex] = val;
                }
            }else{
                wmove(currWindow,(sbHt-2)/2,(sbWd-2)/2);
                wrefresh(currWindow);
                if(vals[currentRowIndex*3 + currentColIndex]!= -1){
                    val = vals[currentRowIndex*3 + currentColIndex];
                    asprintf(&thing, "%d", val);
                }else{
                    print_in_middle(currWindow,(sbHt-2)/2,0,sbWd,thing);
                    
                    print_in_middle(currWindow,(sbHt-2)/2,0,sbWd,"      ");
                }
                wmove(currWindow,(sbHt-2)/2,(sbWd-2)/2);
                wrefresh(currWindow);
            }
            // clear();
            if(overwritten)
                free(thing);
            wmove(stdscr,0,0);
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

    void refreshWindowSet(){
        // Based on tl and br its gonna refresh the cells..
        int cellsCol = cellsInCol;
        int cellsRow = cellsInRow;
        if(tableType == 'l'){
            myfile << "here" << endl;
            cellsRow = 1;
        }
        for(int i = 0; i <cellsCol;i++){
            myfile << "\t";
            for(int j = 0; j<cellsRow;j++){
                char *buff;

                asprintf(&buff, "%d",arr2d[i+top_left.first][j+top_left.second]);
                // snprintf(buff,sizeof(buff),"%d",arr2d[i+top_left.first][j+top_left.second]);
                
                myfile << buff << " ";
                // myfile << "------------Started Formatting--------------" << endl;
                print_in_middle(windowList[i*cellsRow+j],sbHt/2,0,sbWd,"      ");
                print_in_middle(windowList[i*cellsRow+j],sbHt/2,0,sbWd,buff);
                // myfile << "--------------Ended Formatting--------------" << endl;
                // myfile << arr2d[i][j] << endl;
                free(buff);
                wrefresh(windowList[i*cellsRow+j]);

            }
            myfile << endl;
        }
        // For Debugging purposes
        // for(int i = 0; i <cellsInCol;i++){
        //     myfile << "\t";
        //     for(int j = 0; j<cellsInRow;j++){
        //         myfile << arr2d[i+top_left.first][j+top_left.second] << " ";
        //     }
        //     myfile << "\n";
        // }
    }
    void moveUtility(){
        // wbkgd(currWindow,COLOR_PAIR(7));
        int cellsCol = cellsInCol;
        int cellsRow = cellsInRow;
        if(tableType == 'l'){
            // myfile << "here" << endl;
            cellsRow = 1;
        }
        currWindow = windowList[currentRowIndex*cellsRow + currentColIndex];
        wmove(currWindow,0,0);
        // wbkgd(currWindow,COLOR_PAIR(2));
        refresh();
        wrefresh(currWindow);
    }

};

int main(){

    initscr();
    
    cbreak();
    noecho();
    myfile.open ("output.txt");
    
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
    // bounds* bdMainBox = new bounds(19,35,5,10);

    refresh();
    // wrefresh(mainBox);
    
    int cellsPerRow = 3,cellsPerColumn = 3;
    int rows=10,columns=10;    
    // int sbHt = (bdMainBox->height)/cellsPerRow,sbWd = (bdMainBox->width)/cellsPerColumn;
    int ColorVal = 0;
    WINDOW **SubBoxes = new WINDOW*[cellsPerRow*cellsPerColumn];
    WINDOW **labelSB = new WINDOW*[3];

    
    // wattron(stdscr,COLOR_PAIR(1));
    // int k =1; 
    // for(int i=6; i<20; i=i+rows){
    //     wmove(stdscr,i,0);
    //     wprintw(stdscr,"Stduent %d",k);
    //     k++;
    //     refresh();
    //     wrefresh(stdscr);
    // }
    // wattroff(stdscr,COLOR_PAIR(1));
    
    // // k=1;
    // wattron(stdscr,COLOR_PAIR(1));
    // for(int i=11; i<37; i=i+8){
    //     wmove(stdscr,3,i);
    //     wprintw(stdscr,"Prof %d",k);
    //     k++;
    //     refresh();
    //     wrefresh(stdscr);
    // }

    // wattroff(stdscr,COLOR_PAIR(1));
    pair<int,int> tl;
    tl.first = 5;
    tl.second = 20;

    int ht = sbHt;
    int wd = sbWd;
    for(int i=0;i<cellsPerRow;i++){

        for(int j=0;j<cellsPerColumn;j++){
            int rs = tl.first;

            int cs = tl.second;

            SubBoxes[i*cellsPerRow + j] = newwin(ht,wd,tl.first,tl.second);
            tl.second += wd + 1;
            box(SubBoxes[i*cellsPerRow + j],0,0); 
            refresh();
            wrefresh(SubBoxes[i*cellsPerRow + j]);
        }       
        tl.first += ht + 1;
        tl.second = 20;
    }
    
    tl.first = 5;
    tl.second = 5;

    for(int i=0;i<3;i++){

        int rs = tl.first;

        int cs = tl.second;

        labelSB[i] = newwin(ht,wd,tl.first,tl.second);
        box(labelSB[i],0,0); 
        refresh();
        wrefresh(labelSB[i]);
        // for(int j=0;j<1;j++){
        // }       
        tl.first += ht + 1;
        // tl.second = 5;
    }
    
    // int mRow=0;
    char c;

    Control *controlPtr = new Control(SubBoxes,rows,columns,sbHt,sbWd,'a');
    Control *controlPtr2 = new Control(labelSB,10,1,sbHt,sbWd,'l');
    while((c=getch()) != 'q'){
        controlPtr2->moveCursor(c);
        controlPtr->moveCursor(c);
        // if(!){
            
        //     // controlPtr->changeMode(c);
        //     // if(){
        //     //     //  controlPtr->writeText(c);
        //     // }
        // }
    }
    getch();
    myfile.close();
    endwin();
    return 0;
}
