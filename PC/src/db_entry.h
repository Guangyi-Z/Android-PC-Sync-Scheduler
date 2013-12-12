#ifndef DB_ENTRY
#define DE_ENTRY
#include"entry.h"

bool db_addEntry(struct Entry _e);
bool db_delEntry(char* _id);
bool db_modEntry(struct Entry*e );
void db_close();
int db_getEntryList(struct Entry* l, char* date);

#endif
