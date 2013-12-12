#ifndef DISPLAY
#define DISPLAY

#include<curses.h>
#include"entry.h"

#define ROW_MAX 23
#define COL_MAX 78

#define XCALENDAR 0
#define YCALENDAR 0
#define NROW_CALENDAR 20
#define NCOL_CALENDAR 39

#define XLIST 39
#define YLIST 0
#define NROW_LIST 20
#define NCOL_LIST 39

#define XCOMM 0 
#define YCOMM 22
#define NROW_COMM 1
#define NCOL_COMM COL_MAX

#define XALERT 0
#define YALERT 23
#define NROW_ALERT 1
#define NCOL_ALERT COL_MAX

WINDOW* winCal;
WINDOW* winList;
WINDOW* winComm;
WINDOW* winAlert;

/* Command Display */
bool getCommWithTip(const char*, const char*);
bool getModifiedInput(char* , char* , char*);

/* Alert Display */
void printAlert(char* );

/* Calendar Display */
void drawCalendar();

/* Entrylist Display */
bool getEntry(int _num, struct Entry* _e);
void emptyEntryList();
void drawEntryList();

#endif
