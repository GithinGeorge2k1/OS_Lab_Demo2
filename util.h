#ifndef util
#define util

#include <ncurses.h>

void print_in_middle(WINDOW *win, int starty, int startx, int width,const char *string);

// struct bounds{
//     int width,height,rowStart,colStart;

//     bounds(int ht,int wd,int rs,int cs);
// };


// WINDOW* createWin(struct bounds *bd);   
#endif
