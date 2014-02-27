#include <locale.h>
#include <ncurses.h>
#include <wchar.h>


WINDOW *create_newwin(int height, int width, int starty, int startx);
void destroy_win(WINDOW *local_win);

int main(int argc, char *argv[])
{	WINDOW *my_win;
	int startx, starty, width, height;
	int ch;
	wint_t wbuf[128];

	static_assert(sizeof(wchar_t)==sizeof(wint_t), "ERR: wchar_t and wint_t are not convertible.");

	printf("ERR == %08x\n", ERR);
	return 0;

	setlocale(LC_ALL, "");

	initscr();			/* Start curses mode 		*/
	cbreak();			/* Line buffering disabled, Pass on*/
	curs_set(0);
//					 * everty thing to me 		*/
	keypad(stdscr, TRUE);		/* I need that nifty F1 	*/

	height = 3;
	width = 10;
	starty = (LINES - height) / 2;	/* Calculating for a center placement */
	startx = (COLS - width) / 2;	/* of the window		*/
	printw("Press F1 to exit");
	refresh();
	my_win = create_newwin(height, width, starty, startx);

	while((ch = getch()) != KEY_F(1))
	{	switch(ch)
		{	case KEY_LEFT:
				destroy_win(my_win);
				my_win = create_newwin(height, width, starty,--startx);
				break;
			case KEY_RIGHT:
				destroy_win(my_win);
				my_win = create_newwin(height, width, starty,++startx);
				break;
			case KEY_UP:
				destroy_win(my_win);
				my_win = create_newwin(height, width, --starty,startx);
				break;
			case KEY_DOWN:
				destroy_win(my_win);
				my_win = create_newwin(height, width, ++starty,startx);
				break;	
			case KEY_F(3):
				move(LINES - 1, 0);
				printw(":");
				curs_set(1);
				nocbreak();
				fflush(stdin);
				echo();
				refresh();
				get_wstr(wbuf);
				cbreak();
				noecho();
				curs_set(0);
				refresh();
				move(3, 0);
				addwstr((wchar_t *) wbuf);
				refresh();
				break;
		}
	}

	clear();
	move(0, 0);

	printw("Upper left corner           "); addwstr(L"\u2554"); printw("\n"); 
	printw("Lower left corner           "); addch(ACS_LLCORNER); printw("\n");
	printw("Lower right corner          "); addch(ACS_LRCORNER); printw("\n");
	printw("Tee pointing right          "); addch(ACS_LTEE); printw("\n");
	printw("Tee pointing left           "); addch(ACS_RTEE); printw("\n");
	printw("Tee pointing up             "); addch(ACS_BTEE); printw("\n");
	printw("Tee pointing down           "); addch(ACS_TTEE); printw("\n");
	printw("Horizontal line             "); addch(ACS_HLINE); printw("\n");
	printw("Vertical line               "); addch(ACS_VLINE); printw("\n");
	printw("Large Plus or cross over    "); addch(ACS_PLUS); printw("\n");
	printw("Scan Line 1                 "); addch(ACS_S1); printw("\n");
	printw("Scan Line 3                 "); addch(ACS_S3); printw("\n");
	printw("Scan Line 7                 "); addch(ACS_S7); printw("\n");
	printw("Scan Line 9                 "); addch(ACS_S9); printw("\n");
	printw("Diamond                     "); addch(ACS_DIAMOND); printw("\n");
	printw("Checker board (stipple)     "); addch(ACS_CKBOARD); printw("\n");
	printw("Degree Symbol               "); addch(ACS_DEGREE); printw("\n");
	printw("Plus/Minus Symbol           "); addch(ACS_PLMINUS); printw("\n");
	printw("Bullet                      "); addch(ACS_BULLET); printw("\n");
	printw("Arrow Pointing Left         "); addch(ACS_LARROW); printw("\n");
	printw("Arrow Pointing Right        "); addch(ACS_RARROW); printw("\n");
	printw("Arrow Pointing Down         "); addch(ACS_DARROW); printw("\n");
	printw("Arrow Pointing Up           "); addch(ACS_UARROW); printw("\n");
	printw("Board of squares            "); addch(ACS_BOARD); printw("\n");
	printw("Lantern Symbol              "); addch(ACS_LANTERN); printw("\n");
	printw("Solid Square Block          "); addch(ACS_BLOCK); printw("\n");
	printw("Less/Equal sign             "); addch(ACS_LEQUAL); printw("\n");
	printw("Greater/Equal sign          "); addch(ACS_GEQUAL); printw("\n");
	printw("Pi                          "); addch(ACS_PI); printw("\n");
	printw("Not equal                   "); addch(ACS_NEQUAL); printw("\n");
	printw("UK pound sign               "); addch(ACS_STERLING); printw("\n");

	refresh();
	getch();
	endwin();			/* End curses mode		  */

	printf("int value = 0x%x\n", ACS_ULCORNER);

	return 0;
}

WINDOW *create_newwin(int height, int width, int starty, int startx)
{	WINDOW *local_win;

	local_win = newwin(height, width, starty, startx);
	box(local_win, L'\u2551' , 0);		/* 0, 0 gives default characters 
//					 * for the vertical and horizontal
//					 * 					 * lines			*/
//	wborder(local_win, L'\u2551', L'\u2551', L'\u2550', L'\u2550', L'\u2554', L'\u2557', L'\u255a', L'\u255d');
	wrefresh(local_win);		/* Show that box 		*/

	return local_win;
}

void destroy_win(WINDOW *local_win)
{	
	/* box(local_win, ' ', ' '); : This won't produce the desired
 * 	 * result of erasing the window. It will leave it's four corners 
 * 	 	 * and so an ugly remnant of window. 
 * 	 	 	 */
	wborder(local_win, ' ', ' ', ' ',' ',' ',' ',' ',' ');
	/* The parameters taken are 
 * 	 * 1. win: the window on which to operate
 * 	 	 * 2. ls: character to be used for the left side of the window 
 * 	 	 	 * 3. rs: character to be used for the right side of the window 
 * 	 	 	 	 * 4. ts: character to be used for the top side of the window 
 * 	 	 	 	 	 * 5. bs: character to be used for the bottom side of the window 
 * 	 	 	 	 	 	 * 6. tl: character to be used for the top left corner of the window 
 * 	 	 	 	 	 	 	 * 7. tr: character to be used for the top right corner of the window 
 * 	 	 	 	 	 	 	 	 * 8. bl: character to be used for the bottom left corner of the window 
 * 	 	 	 	 	 	 	 	 	 * 9. br: character to be used for the bottom right corner of the window
 * 	 	 	 	 	 	 	 	 	 	 */
	wrefresh(local_win);
	delwin(local_win);
}

