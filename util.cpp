#include "util.h"
#include <string.h>

void print_in_middle(WINDOW *win, int starty, int startx, int width,const char *string)
{	int length, x, y;
	float temp;

	if(win == NULL)
		win = stdscr;
	getyx(win, y, x);
	if(startx != 0)
		x = startx;
	if(starty != 0)
		y = starty;
	if(width == 0)
		width = 80;

	length = strlen(string);
	temp = (width - length)/ 2;
	x = startx + (int)temp;
	mvwprintw(win, y, x, "%s", string);
	wrefresh(win);

    // refresh();
}

// bounds::bounds(int ht,int wd,int rs,int cs) : width(wd),height(ht), rowStart(rs), colStart(cs) {

// }


// WINDOW* createWin(struct bounds *bd){

//     return newwin(bd->height,bd->width,bd->rowStart,bd->colStart);
// }