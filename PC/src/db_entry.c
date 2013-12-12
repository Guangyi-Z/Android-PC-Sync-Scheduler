#include<sqlite3.h>
#include<string.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdbool.h>
#include"entry.h"

#define SQL_MAX_LEN 500
sqlite3 *db;
char* zErrMsg;
char* tableName;

struct Entry* le;
int nle;

void db_initial(){
	int rc;
	
	rc= sqlite3_open("scheduler.db", &db);
	if( rc ){
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		exit(0);
	}

	// create table
	tableName= "schedule";

	char sql[SQL_MAX_LEN];
	memset(sql, '\0', SQL_MAX_LEN);
	strcat(sql, "CREATE TABLE ");
	strcat(sql, tableName);
	strcat(sql, " (id TEXT PRIMARY KEY, "\
			   "title TEXT,"\
			   "info TEXT,"\
			   "date TEXT,"\
			   "beginTime TEXT,"\
			   "endTime TEXT,"\
			   "syncFlag INTEGER"\
			   ");");
	rc = sqlite3_exec(db, sql, NULL, 0, &zErrMsg);
	if( rc != SQLITE_OK ) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
}

void db_close(){
	sqlite3_close(db);
}

bool db_addEntry(struct Entry e){
	if( db == NULL )
		db_initial();
	char sql[SQL_MAX_LEN];
	memset(sql, '\0', SQL_MAX_LEN);
	strcat(sql, "INSERT INTO ");
	strcat(sql, tableName);
	strcat(sql, " VALUES('");
	strcat(sql, e.id);
	strcat(sql, "', '");
	strcat(sql, e.title);
	strcat(sql, "', '");
	strcat(sql, e.info);
	strcat(sql, "', '");
	strcat(sql, e.date);
	strcat(sql, "', '");
	strcat(sql, e.beginTime);
	strcat(sql, "', '");
	strcat(sql, e.endTime);
	strcat(sql, "', ");
	char sync[2];
	sprintf(sync, "%d", e.syncFlag);
	strcat(sql, sync);
	strcat(sql, ");");

	int rc= sqlite3_exec(db, sql, NULL, NULL, &zErrMsg);
	if( SQLITE_OK != rc ){
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
		return false;
	}
	return true;
}

bool db_modEntry(struct Entry* e ){
	if( db == NULL )
		db_initial();
	char sql[SQL_MAX_LEN];
	memset(sql, '\0', SQL_MAX_LEN);
	strcat(sql, "UPDATE ");
	strcat(sql, tableName);
	strcat(sql, " SET title='");
	strcat(sql, e->title);
	strcat(sql, "', info='");
	strcat(sql, e->info);
	strcat(sql, "', beginTime='");
	strcat(sql, e->beginTime);
	strcat(sql, "', endTime='");
	strcat(sql, e->endTime);
	strcat(sql, "', syncFlag='");
	char sync[2];
	sprintf(sync, "%d", e->syncFlag);
	strcat(sql, sync);
	strcat(sql, "' WHERE id='");
	strcat(sql, e->id);
	strcat(sql, "';");

	int rc= sqlite3_exec(db, sql, NULL, NULL, &zErrMsg);
	if( SQLITE_OK != rc ){
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
		return false;
	}
	return true;
}

bool db_delEntry(char* _id){
	if( db == NULL )
		db_initial();
	char sql[SQL_MAX_LEN];
	memset(sql, '\0', SQL_MAX_LEN);
	strcat(sql, "DELETE FROM ");
	strcat(sql, tableName);
	strcat(sql, " WHERE id = '");
	strcat(sql, _id);
	strcat(sql, "';");
	int rc=sqlite3_exec(db, sql, NULL, NULL, &zErrMsg);
	if( SQLITE_OK != rc ){
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
		return false;
	}
	return true;
}

static int cb_getEntryList(void *l, int argc, char **argv, char **azColName);

int db_getEntryList(struct Entry* l, char* date){
	if( db == NULL )
		db_initial();
	char sql[SQL_MAX_LEN];
	memset(sql, '\0', SQL_MAX_LEN);
	strcat(sql, "SELECT * FROM ");
	strcat(sql, tableName);
	strcat(sql, " WHERE date = '");
	strcat(sql, date);
	strcat(sql, "';");
	nle= 0;	// first set the number of entries to 0
	int rc=sqlite3_exec(db, sql, cb_getEntryList, (void*)l, &zErrMsg);
	if( SQLITE_OK != rc ){
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	return nle;
}

static int cb_getEntryList(void *l, int argc, char **argv, char **azColName){
	struct Entry* le= (struct Entry*)l;
	int i;
	for(i=0; i<argc; i++){
		if( 0 == strcmp(azColName[i], "id") ){
			strcpy( le[nle].id, argv[i]?argv[i]:"NULL" );
		} else if( 0 == strcmp(azColName[i], "title") ){
			strcpy( le[nle].title, argv[i]?argv[i]:"NULL" );
		} else if( 0 == strcmp(azColName[i], "info") ){
			strcpy( le[nle].info, argv[i]?argv[i]:"NULL" );
		} else if( 0 == strcmp(azColName[i], "beginTime") ){
			strcpy( le[nle].beginTime, argv[i]?argv[i]:"NULL" );
		} else if( 0 == strcmp(azColName[i], "endTime") ){
			strcpy( le[nle].endTime, argv[i]?argv[i]:"NULL" );
		} else if( 0 == strcmp(azColName[i], "syncFlag") ){
			int sync= 0;
			if( !argv[i] ){
				sync= argv[i][0] - '0';
			}
			le[nle].syncFlag= sync;
		} else if( 0 == strcmp(azColName[i], "date") ){
			// already set
		}
	}
	nle++;	// increase the number of entries by 1
	return 0;
}

static int callback(void *NotUsed, int argc, char **argv, char **azColName){
	int i;
	for(i=0; i<argc; i++){
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}
