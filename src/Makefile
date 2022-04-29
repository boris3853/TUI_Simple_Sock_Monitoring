.PHONY :  menu menu2 form

cc = gcc -o
MON = menu2.c JSON_TO_CSV.c
MON_L = -ljson-c
FORM = form.c get_list.c get_Delay_Data.c
FORM_L = -lcurl -lform
MENU_L = -lcurses -lmenu

all:  menu

menu:
	$(cc) menu menu.c $(FORM) $(MON) $(MON_L) $(FORM_L) $(MENU_L)
