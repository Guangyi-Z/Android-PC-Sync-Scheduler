#ifndef ENTRY
#define ENTRY

/* define length of each items in Entry structure */
#define LEN_ID 10
#define LEN_TITLE 100 
#define LEN_INFO 300
#define LEN_DATE 12
#define LEN_BEGINTIME 7
#define LEN_ENDTIME 7

/* STRUCTURE */
struct Entry {
	char id[LEN_ID];
	char title[LEN_TITLE];
	char info[LEN_INFO];
	char date[LEN_DATE];
	char beginTime[LEN_BEGINTIME];
	char endTime[LEN_ENDTIME];
	int syncFlag;
};

// flag for syncFlag in Entry item
#define SYNC_UNCHANGED 0
#define SYNC_NEW 1
#define SYNC_MODIFIED 2

#endif
