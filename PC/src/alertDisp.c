#include"display.h"

void initialAlert(){
	winAlert= newwin(NROW_ALERT, NCOL_ALERT, YALERT, XALERT);
}

void printAlert(char* msg){
	if( NULL == winAlert )
		initialAlert();
	wclear(winAlert);
	mvwprintw(winAlert, XALERT, YALERT, "%s", msg);
	wrefresh(winAlert);
}
