/*
** an entry list which containing the schedule entries of specific day is shown centrally in the program
*/
#include<curses.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include<time.h>
#include"calendar.h"
#include"entry.h"
#include"db_entry.h"

/*
** return the length of the array of Entry items
*/
int getEntryList(struct Entry* l, int y, int m, int d){
	char date[LEN_DATE];
	memset(date, '\0', LEN_DATE);
	sprintf(date, "%d-%d-%d", y+2000, m, d);
	return db_getEntryList(l, date);
}

bool delEntry(struct Entry* _e){
	return db_delEntry(_e->id);
}

bool addEntry(char* title, char* info, char* date, char* beginTime, char* endTime) {	
	struct Entry e;
	srand(time(NULL));
	char id[10];
	sprintf(id, "ID_%d", (rand() % 100000) );	// id range(0,100000)
	strcpy(e.id, id);
	strcpy(e.title, title);
	strcpy(e.info, info);
	strcpy(e.date, date);
	strcpy(e.beginTime, beginTime);
	strcpy(e.endTime, endTime);
	e.syncFlag= SYNC_NEW;

	return db_addEntry(e);
}

bool modEntry(struct Entry* e){
	return db_modEntry(e);
}
