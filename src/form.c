#include <form.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

#define SIZE(a) (sizeof(a)/sizeof(a[0]))



extern char Ticker_item[25][10];
void get_list();
void get_json(int, int, int, int);

enum form_elements{
	f_y, f_m, f_d,
	f_ITV
};

void json_sig(int num){
	static int cnt = 19;
	mvprintw(LINES - 8, cnt++, ">");
	refresh();
}

void form(){
	FIELD *field[5];
	FORM *form;

	char str[10];
	int c;
	int y, m, d, ITV;

	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);

	// SIGNAL
	void json_sig(int);
	struct sigaction json_sa;
	json_sa.sa_handler = json_sig;
	json_sa.sa_flags = 0;
	sigemptyset(&json_sa.sa_mask);
	sigaction(SIGUSR1, &json_sa, NULL);

	field[f_y] = new_field(1, 4, 6, 26, 0, 0);	// Y
	field[f_m] = new_field(1, 2, 6, 31, 0, 0);	// M
	field[f_d] = new_field(1, 2, 6, 34, 0, 0);	// D
	field[f_ITV] = new_field(1, 2, 8, 26, 0, 0);
	field[4] = NULL;

	for(int i=0;i<SIZE(field);++i)
		set_field_back(field[i], A_UNDERLINE);

	set_field_type(field[f_y], TYPE_INTEGER,0,1970,2021);
	set_field_type(field[f_m], TYPE_INTEGER,2,1,12);
	set_field_type(field[f_d], TYPE_INTEGER,2,1,31);
	set_field_type(field[f_ITV], TYPE_INTEGER,2,1,15);
	
	form = new_form(field);
	post_form(form);


	mvprintw(6, 10, "1.Date  : ");	
	mvprintw(6, 30, ".");
	mvprintw(6, 33, ".");
	mvprintw(8, 10, "2.Interval : ");
	mvprintw(8, 29, "min");

	for(int i=0;i<COLS;++i){
		mvprintw(LINES-6,i,"=");
		mvprintw(LINES-1,i,"=");
	}

	mvprintw(LINES - 5, 0, "Data is available 1min(10 Days) / 5min (18days) / 15min (18days)");
	mvprintw(LINES - 4, 0, "Interval : 1min / 5min / 15min");
        mvprintw(LINES - 3, 0, "Left and Right arrow keys to move cursor (Backspace to delete)");
	mvprintw(LINES - 2, 0, "Press <F1> to Go Back to Main Menu");
	pos_form_cursor(form);
	refresh();

	
	while((c=getch()) != KEY_F(1)){
		switch(c){
			case KEY_RIGHT:
				form_driver(form, REQ_NEXT_FIELD);
				form_driver(form, REQ_END_LINE);
				break;
			case KEY_LEFT:
				form_driver(form, REQ_PREV_FIELD);
				form_driver(form, REQ_END_LINE);
				break;
			case KEY_BACKSPACE:
				form_driver(form, REQ_CLR_FIELD);
				break;
			case 10: //Enter	
				form_driver(form, REQ_NEXT_FIELD);
				y = atoi(field_buffer(field[f_y],0));
				m = atoi(field_buffer(field[f_m],0));
				d = atoi(field_buffer(field[f_d],0));
				ITV = atoi(field_buffer(field[f_ITV],0));
				
				mvprintw(LINES - 7, 0, "                   0                      25");			
				mvprintw(LINES - 8, 0, "                     ");
				mvprintw(LINES - 8, 0, "Getting JSON FILES[                         ]");
				move(LINES - 8 , 19);

				refresh();
				get_json(y, m, d, ITV);
				sleep(1);
				mvprintw(LINES - 7, 0, "                                              ");			
				mvprintw(LINES - 8, 0, "                                              ");
				refresh();
				mvprintw(LINES - 8, 0, "Done...");
				refresh();
				
				break;
			default:
				form_driver(form, c);
				break;
		}
	}

	unpost_form(form);
	free(form);
	for(int i=0;i<SIZE(field);++i)
		free_field(field[i]);
	endwin();
}

