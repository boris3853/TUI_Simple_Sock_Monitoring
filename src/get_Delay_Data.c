#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <sys/types.h>
#include <signal.h>


#define SIZE(a) (sizeof(a)/sizeof(a[0]))

extern char Ticker_item[25][10];

void get_list();

void get_json(int Y, int M, int D, int ITV){
	FILE *fp;	
	char URI[200] = "";
	char *p1 = "https://sandbox.tradier.com/v1/markets/timesales?symbol=";
	char *p2 = "&interval=";
	char p3[20];
	char *p4 = "min&start=";
	char p5[20];
	char *p6 = " 09:30&end=";
	char p7[20];
	char *p8 = " 16:00&session_filter=all";
	
	char out[20] = "";

	sprintf(p3, "%d", ITV);
	sprintf(p5, "%d-%d-%d", Y, M, D);
	strcpy(p7, p5);
	


	get_list();

	for(int i=0;i<(int)SIZE(Ticker_item);++i){
		strcpy(URI, "");
		strcpy(out, "");
		CURL *curl = curl_easy_init();
		struct curl_slist *list = NULL;
		if(curl){
			strcat(URI, p1);
			strcat(URI, Ticker_item[i]);
			strcat(URI, p2);
			strcat(URI, p3);
			strcat(URI, p4);
			strcat(URI, p5);
			strcat(URI, p6);
			strcat(URI, p7);
			strcat(URI, p8);

		
			strcat(out, "data/JSON/");
			strcat(out, Ticker_item[i]);
			strcat(out, ".json");
			
			
			fp = fopen(out, "w");
			
			curl_easy_setopt(curl, CURLOPT_URL, URI);
			curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

			list = curl_slist_append(list, "Authorization: Bearer zLD0GP1x6ZP2dbKw0R44TgFc1Y0l");
			list = curl_slist_append(list, "Accept: application/json");

			curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);

			curl_easy_perform(curl);

			curl_slist_free_all(list);
			fclose(fp);
			
			pid_t pid = fork();
			if(!pid){
				kill(getpid(), SIGUSR1);
				exit(0);
			}	
		}
	}
}
