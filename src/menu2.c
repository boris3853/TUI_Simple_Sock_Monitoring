#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <menu.h>
#include <string.h>

// ticker_menu_win parameter
#define T_HEIGHT 20
#define T_WIDTH	10

#define SIZE(a) (sizeof(a)/sizeof(a[0]))
extern char Ticker_item[25][10];
char table_item_buf[391][80];

extern void get_list();
extern int json_to_csv();

void monitor(){
	// ITEM, MENU variable
	ITEM **ticker_item, **table_item;
	MENU *ticker_menu, *table_menu;
	WINDOW *ticker_menu_win, *table_menu_win;

	int n_ticker, c, flag = 1;
	int n_table;
	int startx, starty;
	int G_WIDTH, G_HEIGHT;
	
	get_list();
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);

	// ticker_menu_win setup
	n_ticker = SIZE(Ticker_item);
	ticker_item = (ITEM **)calloc(n_ticker+1, sizeof(ITEM *));

	for(int i=0;i<n_ticker;++i)
		ticker_item[i] = new_item(Ticker_item[i], "");
	ticker_menu = new_menu((ITEM **)ticker_item);

	ticker_menu_win = newwin(T_HEIGHT,T_WIDTH,0,0);
        keypad(ticker_menu_win, TRUE);


	set_menu_win(ticker_menu, ticker_menu_win);
	set_menu_sub(ticker_menu, derwin(ticker_menu_win, T_HEIGHT-4, T_WIDTH-2,3,1));
	box(ticker_menu_win, 0, 0);
	set_menu_mark(ticker_menu, " > ");
	mvwprintw(ticker_menu_win,1,1,"%s"," Ticker ");
	mvwprintw(ticker_menu_win,2,1,"%s","========");
	
	mvprintw(LINES - 3, 0, "Right and Left arrow keys to change Menu");
	mvprintw(LINES - 2, 0, "Up and Down arrow keys to naviagte (F1 to Exit)");

	post_menu(ticker_menu);
	refresh();
	wrefresh(ticker_menu_win);
	


	// table_menu_win setup
	G_WIDTH = COLS - T_WIDTH-1;
	G_HEIGHT = T_HEIGHT;

	n_table = json_to_csv();
	table_item = (ITEM **)calloc(n_table+1, sizeof(ITEM *));	

	for(int i=0;i<n_table;++i)
		table_item[i] = new_item("", "");
	table_menu = new_menu((ITEM **)table_item);

	
	table_menu_win = newwin(G_HEIGHT, G_WIDTH, 0, T_WIDTH+1);

	set_menu_win(table_menu, table_menu_win);
	set_menu_sub(table_menu, derwin(table_menu_win, G_HEIGHT-4, G_WIDTH-2, 3,1));
	box(table_menu_win, 0, 0);
	set_menu_mark(table_menu, "  ==>  ");
		
	post_menu(table_menu);
	
	for(int i=1;i<G_WIDTH-1;++i)
		mvwprintw(table_menu_win, 2, i, "=");
	refresh();
	wrefresh(table_menu_win);	

	
	while((c=wgetch(ticker_menu_win)) != KEY_F(1))
	{
		switch(c)
		{
			case KEY_DOWN:
				menu_driver(ticker_menu, REQ_DOWN_ITEM);
				break;
			case KEY_UP:
				menu_driver(ticker_menu, REQ_UP_ITEM);
				break;
			case KEY_RIGHT: // Enter
			{
				
				mvwprintw(table_menu_win, 1, 1, "     ");
				mvwprintw(table_menu_win, 1, 1, "%5s | ", 
						item_name(current_item(ticker_menu)));	
				mvwprintw(table_menu_win, 1, 8, "%10s", "date");
				mvwprintw(table_menu_win, 1, 19, "%8s", "time");
				mvwprintw(table_menu_win, 1, 28, "%6s", "open");
				mvwprintw(table_menu_win, 1, 35, "%6s", "low");
				mvwprintw(table_menu_win, 1, 42, "%6s", "high");
				mvwprintw(table_menu_win, 1, 49, "%6s", "close");
				mvwprintw(table_menu_win, 1, 58, "%6s", "volume");

				wrefresh(table_menu_win);
				
				char URI[200];
				FILE *fp;
				strcpy(URI, "data/CSV/");
				strcat(URI, item_name(current_item(ticker_menu)));
				strcat(URI, ".csv");
				
				keypad(ticker_menu_win, FALSE);
				keypad(table_menu_win, TRUE);
				
				unpost_menu(table_menu);
				
				fp = fopen(URI, "r"); 
				
				for(int i=0;i<n_table;++i){
					char date[20]="", time[20]="";
					double open=0, low=0, high=0, close=0;
					int volume=0;
					strcpy(table_item_buf[i], "");

					fscanf(fp, "%s %s %lf %lf %lf %lf %d", 
							date, time, &open, &low, &high, 
							&close, &volume);
					sprintf(table_item_buf[i], "%s %s %.2f %.2f %.2f %.2f %8d",
							date, time, open, low, high,
							close, volume);
					
					table_item[i] = new_item(table_item_buf[i], "");
				
				}	
				fclose(fp);

				set_menu_items(table_menu, (ITEM **)table_item);
				post_menu(table_menu);
				
				refresh();
				wrefresh(table_menu_win);
				
				while((c=wgetch(table_menu_win)) != KEY_F(1) && flag)
				{
					switch(c){
						case KEY_DOWN:
							menu_driver(table_menu, REQ_DOWN_ITEM);
							break;
						case KEY_UP:
							menu_driver(table_menu, REQ_UP_ITEM);
							break;
						case KEY_LEFT:
							flag = 0;
							break;
					}
				}
			
				flag = 1;
				keypad(table_menu_win, FALSE);
				keypad(ticker_menu_win, TRUE);
				break;
			}
				
		}
	}

	unpost_menu(ticker_menu);
	for(int i=0;i<n_ticker;++i)
		free_item(ticker_item[i]);
	free_menu(ticker_menu);

	unpost_menu(table_menu);
	for(int i=0;i<n_table;++i)
		free_item(table_item[i]);
	free_menu(table_menu);
	endwin();
}
