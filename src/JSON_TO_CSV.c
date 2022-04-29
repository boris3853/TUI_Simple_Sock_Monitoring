#include <stdio.h>
#include <string.h>
#include <json-c/json.h>

#define SIZE(a) (sizeof(a)/sizeof(a[0]))

extern char Ticker_item[25][10];
void get_list();
void json_sig(int);

int json_to_csv(){
	char URI1[200] = "";
	char URI2[200] = "";
	int size;

	get_list();
	for(int i=0;i<(int)SIZE(Ticker_item);++i){
		FILE *fp;
		strcpy(URI1, "");
		strcat(URI1, "data/JSON/");
		strcat(URI1, Ticker_item[i]);
		strcat(URI1, ".json");

		strcpy(URI2, "");
		strcat(URI2, "data/CSV/");
		strcat(URI2, Ticker_item[i]);
		strcat(URI2, ".csv");

		fp = fopen(URI2, "w");
		
		json_object *root = json_object_from_file(URI1);
		json_object *s_data = json_object_object_get(root, "series");
		json_object *d_data = json_object_object_get(s_data, "data");
		
		int length = json_object_array_length(d_data);

		for(int i=0;i<length;++i){
			json_object *obj = json_object_array_get_idx(d_data, i);

			json_object *time = json_object_object_get(obj, "time");
			json_object *open = json_object_object_get(obj, "open");
			json_object *high = json_object_object_get(obj, "high");
			json_object *low = json_object_object_get(obj, "low");
			json_object *close = json_object_object_get(obj, "close");
			json_object *volume = json_object_object_get(obj, "volume");
			char *time_s = strchr(json_object_get_string(time), 'T');
			*time_s = ' ';
			fprintf(fp, "%s %s %s %s %s %s\n",
			json_object_get_string(time) , json_object_get_string(open), 
			json_object_get_string(low), json_object_get_string(high), 
			json_object_get_string(close), json_object_get_string(volume));

		}
		fclose(fp);

		size = length;
	}

	return size;
}
