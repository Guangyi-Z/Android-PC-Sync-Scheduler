#include"display.h"
#include"calendar.h"

int nCubeHeight= 1,
	nCubeWidth= 3;
char* title= "Calendar";

void initialCalWin(){
	winCal= newwin(NROW_CALENDAR, NCOL_CALENDAR, YCALENDAR, XCALENDAR);
}

//* destroy

void drawCalendar(){
	if( NULL == winCal )
		initialCalWin();
	int x= XCALENDAR,
		y= YCALENDAR;
	int *aDays= NULL;
	int aDays_len= getCalendarArray(&aDays);
	wclear(winCal);
	wmove(winCal, 0, 0);
	mvwprintw(winCal, x, y, "%s", title);	// print title in left top
	x+= nCubeHeight;
	for( int i= 0; i<aDays_len; i++ ){
		if( i%7 == 0 && i != 0 )
			x+= nCubeHeight;
		y= YCALENDAR + (i%7)*nCubeWidth;
		mvwprintw(winCal, x, y, "%d", aDays[i]);
	}
	wrefresh(winCal);
}


