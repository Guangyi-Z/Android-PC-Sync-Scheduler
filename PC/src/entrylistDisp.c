#include<string.h>
#include"display.h"
#include"entrylist.h"


struct Entry le[11];	// at most 11 entry a day
int nle;	// length of entries

int nEntryHeight= 2;	// each entry takes up 2 lines within the winList

void initialEntrylistWin(){
	winList= newwin(NROW_LIST, NCOL_LIST, YLIST, XLIST);
}

void emptyEntryList(){
	if( winList == NULL )
		initialEntrylistWin();
	nle= 0;	// empty the entry list
	wclear(winList);
}

/*
** return 1 if succees, 0 if fail
*/
bool getEntry(int _num, struct Entry* _e){
	if(_num <= nle){
		memcpy(_e, &(le[_num-1]), sizeof(struct Entry));
		return true;
	}
	else return false;
}

void drawEntryList(int y, int m, int d){
	emptyEntryList();
	int nle= getEntryList(le, y, m, d);
	for( int i= 0; i< nle; i++ ){
		wmove(winList, i*nEntryHeight, 0);
		wprintw(winList, "%d TITLE: %s	(%s-%s)", i+1, le[i].title, le[i].beginTime, le[i].endTime);
		wmove(winList, i*nEntryHeight+1, 0);
		wprintw(winList, "  INFO: %s", le[i].info);
	}
	wrefresh(winList);
}
