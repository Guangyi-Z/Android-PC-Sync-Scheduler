#include"display.h"
#include"parser.h"
#include"calendar.h"
#include"db_entry.h"
#include"entrylist.h"
#include<stdio.h>
#include<time.h>
#include"sync.h"

#define COMM_MAX_LEN 100
char comm[COMM_MAX_LEN];
char* tipComm= "$> ";

int sys_year,
	sys_month,
	sys_day;
int show_year,
	show_month,
	show_day;


void initialDistributor(){
	// get system current time;
	time_t t= time(NULL);
	struct tm date= *localtime(&t);
	show_year= date.tm_year - 100;
	show_month= date.tm_mon+1;
	show_day= date.tm_mday;
	// set up showing time
	sys_year= show_year;
	sys_month= show_month;
	sys_day= show_day;
	// set up the curses lib
	initscr();
}

bool doMonth(char *comm){
	int n1, n2;
	parseTwoInt(comm, &n1, &n2);
	if( n1 != -1 ){
		if( n2 != -1 ){	// 2 args
			if(! isValidMonth(n1, n2) ){
				return false;
			}
			show_year= n1;
			show_month= n2;
			show_day= -1;
		} else{	// 1 args
			if(! isValidMonth(show_year, n1)){
				return false;
			}
			show_month= n1;
			show_day= -1;
		}
	} else {	// no args
		show_year= sys_year;
		show_month= sys_month;
		show_day= sys_day;
	}
	updateCalendar(show_year, show_month, show_day);
	emptyEntryList();
	drawCalendar();
	//* emptyEntryList();
	return true;
}

bool doModify(char* comm){
	if( show_day == -1 )
		return false;
	int num= parseOneInt(comm);
	if( num == -1 )
		return false;
	struct Entry e;
	getEntry(num, &e);
	char s[300];
	memset(s, '\0', 300);
	if( false == getModifiedInput("title:", e.title, s) )
		return false;
	strcpy(e.title, s);
	if( false == getModifiedInput("info:", e.info, s) )
		return false;
	strcpy(e.info, s);
	if( false == getModifiedInput("begin time:", e.beginTime, s) )
		return false;
	strcpy(e.beginTime, s);
	if( false == getModifiedInput("end time:", e.endTime, s) )
		return false;
	strcpy(e.endTime, s);

	e.syncFlag+= SYNC_MODIFIED;	// update the sync flag
	if( false == modEntry(&e) )
		return false;
	drawEntryList(show_year, show_month, show_day);
	return true;
}

bool doAdd(char* comm){
	if( show_day == -1 ) {
		printAlert("select a day first!");
		return false;
	}
	char title[50];
	char info[100];
	char date[9];
	char beginTime[6];
	char endTime[6];
	// fill the data above
	getCommWithTip("input title:", title);
	getCommWithTip("input content:", info);
	getCommWithTip("input begin time(hh:mm):", beginTime);
	getCommWithTip("input end time(hh:mm):", endTime);
	sprintf(date, "%d-%d-%d", show_year+2000, show_month, show_day);	// date format: "yyyy-MM-dd"
	// add entry
	if( false == addEntry(title, info, date, beginTime, endTime) )
		return false;
	drawEntryList(show_year, show_month, show_day);
	return true;
}

bool doDay(char* comm){
	int n1= parseOneInt(comm);
	if( n1 == -1 ){	// trun to system day
		show_year= sys_year;
		show_month= sys_month;
		show_day= sys_day;
	} else {	// turn to a specific day
		if( isValidDay(show_year, show_month, n1) ) {
			show_day= n1;
		} else {
			return false;
		}
	}
	drawEntryList(show_year, show_month, show_day);
	return true;
}

bool doDel(char* comm){
	if( show_day == -1 )
		return false;
	int num= parseOneInt(comm);
	if( num == -1 )
		return false;
	struct Entry e;
	if( false == getEntry(num, &e) )
		return false;
	if( false == delEntry(&e) )
		return false;
	drawEntryList(show_year, show_month, show_day);
	return true;
}

void distribute(){

	while(1) {
		if( true == getCommWithTip(tipComm, comm) ){
			switch(comm[0]){
			case 'p':
				goPrevDay(&show_year, &show_month, &show_day);
				drawEntryList(show_year, show_month, show_day);
				break;
			case 'n':
				goNextDay(&show_year, &show_month, &show_day);
				drawEntryList(show_year, show_month, show_day);
				break;
			case 'd':
				if( 'e' != comm[1] ){
					if( false == doDay(comm) )
						printAlert("wrong input format");
				}
				else {	// delete entry
					if( false == doDel(comm) )
						printAlert("wrong input format");
				}
				break;
			case 'm':
				if( comm[1] != 'o' ){	// turn to month
					if( false == doMonth(comm) )
						printAlert("wrong input format");
				} else {	// trun to entry modification
					if( false == doModify(comm) )
						printAlert("wrong input format");
				}
				break;
			case 'a':
				if( false == doAdd(comm) )
					printAlert("wrong input format");
				break;
			case 's':
				sync_init();
				sync_close();
				drawEntryList(show_year, show_month, show_day);
				break;
			case 'q':
				// things before exiting
				endwin();
				db_close();
				return;
			default :
				printAlert("wrong input format");
			}	// end switch
		} else {
			printAlert("wrong input format");
		}
	}	// end while

}

