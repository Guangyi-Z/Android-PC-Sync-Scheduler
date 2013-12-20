#ifndef DB_ENTRY
#define DE_ENTRY
#include"entry.h"
#include<stdbool.h>

bool db_addEntry(struct Entry _e);
bool db_delEntry(char* _id);
bool db_modEntry(struct Entry*e );
void db_close();
int db_getEntryList(struct Entry* l, char* date);
bool db_getEntry(struct Entry *e, char* id);
void db_syncEntries();
int db_getAllEntries(struct Entry * l);

#endif
