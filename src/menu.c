#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <menu.h>
#include <string.h>

#include <signal.h>
#include <sys/time.h>
#include <unistd.h>

// ITEM, MENU variable
ITEM **total_item;
MENU *main_menu;
WINDOW *main_menu_win;

// window parameter
#define HEIGHT 8
#define WIDTH 70
#define SIZE(a) (sizeof(a)/sizeof(a[0]))

// form
void form();

// monitor
void monitor();


char *menu_item[] = {
			"1. Get Data",
			"2. Monitoring",
			"3. Exit",
			(char *)NULL
		};
char *title = "Paper Trader Simulator";

int left_time = 10;
void timer(int num){
	mvprintw(LINES - 1, 0, "                                       ");
	mvprintw(LINES - 1, 0, "Automatically selected in %d seconds..", left_time--);
	if(left_time < 0){
		fflush(stdin);
		mvprintw(LINES - 1, 0, "                                       ");
		mvprintw(LINES - 1, 0, "SELECTED");
	}
	refresh();	
}

int set_ticker(int n_msecs)
{
	struct itimerval new_timeset;
	long n_sec, n_usecs;
	n_sec = n_msecs / 1000;
	n_usecs = (n_msecs % 1000) * 1000L;

	new_timeset.it_interval.tv_sec = n_sec;
	new_timeset.it_interval.tv_usec = n_usecs;
	new_timeset.it_value.tv_sec = n_sec;
	new_timeset.it_value.tv_usec = n_usecs;

	return setitimer(ITIMER_REAL, &new_timeset, NULL);
}

void printMain(WINDOW *win, MENU *menu){
	box(win, 0, 0);
	post_menu(menu);
        mvprintw(1, COLS/2 - SIZE(title), "%s", title);
        mvprintw(LINES - 3, 0, "Press <ENTER> to see the option selected");
        mvprintw(LINES - 2, 0, "Right and Left arrow keys to naviagte (F1 to Exit)");
}

int main(){
	int n, c, flag = 1;
	int startx, starty;

	// set up
	initscr();
        cbreak();
        noecho();
        keypad(stdscr, TRUE);
	
	// SIGNAL
	void timer(int);
	signal(SIGALRM, timer);
	set_ticker(500);
	

	int pid;
	if((pid = fork()) == 0){
		while(1)
			pause();
	}
	else{


        // menu setting
	n = SIZE(menu_item);
        total_item = (ITEM **)calloc(n, sizeof(ITEM *));

        for(int i=0;i<n;++i)
                total_item[i] = new_item(menu_item[i], "");
        main_menu = new_menu((ITEM **)total_item);
	
	// main_menu_win setting
	starty = (LINES - HEIGHT) / 2;
	startx = (COLS - WIDTH) / 2;
	main_menu_win = newwin(HEIGHT,WIDTH,starty,startx);
	keypad(main_menu_win, TRUE);

	set_menu_win(main_menu, main_menu_win);
	set_menu_sub(main_menu, derwin(main_menu_win, HEIGHT-4,WIDTH-2,4,1));
	set_menu_format(main_menu, 5, 3);
	set_menu_mark(main_menu, " > ");

	printMain(main_menu_win, main_menu);	
	refresh();
	wrefresh(main_menu_win);

	while(flag)
	{
		wrefresh(main_menu_win);
		refresh();
		c=wgetch(main_menu_win);
		if(left_time < 0) c = 10;
		refresh();
		switch(c)
		{
			case KEY_F(1):
				flag = 0;
				break;
			case KEY_RIGHT:
				left_time = 10;
				menu_driver(main_menu, REQ_RIGHT_ITEM);
				break;
			case KEY_LEFT:
				left_time = 10;
				menu_driver(main_menu, REQ_LEFT_ITEM);
				break;
			case 10: // select
			{
				// setting
				erase();
				signal(SIGALRM, SIG_IGN);

				// Menu Select
				if(!strcmp(item_name(current_item(main_menu)), "1. Get Data")) 
					form();
				if(!strcmp(item_name(current_item(main_menu)), "2. Monitoring"))
					monitor();
				if(!strcmp(item_name(current_item(main_menu)), "3. Exit"))
					flag = 0;
				
				erase();
				printMain(main_menu_win, main_menu);
				refresh();

				//signal restart
				signal(SIGALRM, timer);
				set_ticker(500);
				left_time = 10;
				break;
			}
		}
	}

	unpost_menu(main_menu);
	free_menu(main_menu);
	for(int i=0;i<n;++i)
		free_item(total_item[i]);
	endwin();
	}
}
