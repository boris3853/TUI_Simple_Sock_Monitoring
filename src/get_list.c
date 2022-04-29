#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char Ticker_item[25][10];
char buf[10];

void get_list(){
	FILE *fp;
	int i = 0;
	fp = fopen("list/ticker_list.data", "r");
	while(!feof(fp)){
		fscanf(fp, "%s", buf);
		strcpy(Ticker_item[i++], buf);
	}
}
