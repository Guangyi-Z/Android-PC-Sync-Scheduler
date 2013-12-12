#ifndef ENTRYLIST
#define ENTRYLIST
#include"entry.h"

/* entryList.c */
void emptyEntryList();

int getEntryList(struct Entry*, int y, int m, int d);
bool delEntry(struct Entry*);
bool addEntry(char* title, char* info, char* date, char* beginTime, char* endTime);
bool modEntry(struct Entry* e);

#endif
