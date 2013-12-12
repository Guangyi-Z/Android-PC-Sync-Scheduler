#include"display.h"
#include<string.h>

void initialComm(){
	winComm= newwin(NROW_COMM, NCOL_COMM, YCOMM, XCOMM);
	//winComm= newwin(NROW_COMM, NCOL_COMM, 0, 20);
}

bool getCommWithTip(const char* tip, const char* comm){
	if( NULL == winComm )
		initialComm();
	wmove(winComm, 0, 0);
	wprintw(winComm, "%s", tip);
	wrefresh(winComm);
	int res= wgetnstr(winComm, comm, NCOL_COMM);
	wclear(winComm);
	if( OK == res )
		return true;
	else return false;
}

/*
** show the old string to user, and copy the new input to new string
** return false when give up to get the new input
*/
bool getModifiedInput(char* tip, char* old, char* new){
	if( NULL == winComm )
		initialComm();
	keypad(winComm, true);
	raw();
	nonl();	// disable return key translated to new line in either input or output mode
	noecho();

	wmove(winComm, 0, 0);
	wprintw(winComm, "%s%s", tip, old);
	wrefresh(winComm);
	int xLeft= strlen(tip);	// the very position after the tip string
	int xRight= -1;	// the very position after the whole input
	int ch= '\0';
	int x= 0,
		y= 0;
	strcpy(new, old);
	int new_len= strlen(new);	// the length of the new input string
	while(1){
		ch= wgetch(winComm);
		if(ch == 13)	// press enter to complete
			break;
		if(ch == 27)	// press esc to give up
			return false;
		switch(ch){
			case KEY_BACKSPACE:
				x= getcurx(winComm);
				if( x > xLeft ){
					wmove(winComm, y, x-1);	// delete the char one before the cursor
					wdelch(winComm);
					int cur= x - xLeft;
					for( int i= cur; i>= new_len-1; i-- ){
						new[i-1]= new[i];
					}
					new_len--;
					//wmove(winComm, x--, y);
				}
				break;
			case KEY_LEFT:
				x= getcurx(winComm);
				if(x > xLeft)
					wmove(winComm, y, --x);
				break;
			case KEY_RIGHT:
				x= getcurx(winComm);
				xRight= xLeft + new_len;
				if(x < xRight)
					wmove(winComm, y, ++x);
				break;
			default:
				x= getcurx(winComm);
				xRight= xLeft + new_len;
				if(x == xRight){	// append the ch to the end of new string
					new[new_len++]= ch;
					//wmove(winComm, y, ++x);
					waddch(winComm, ch);
				} else {	// insert the ch before current cursor
					int cur= x-xLeft;
					for( int i= new_len; i> cur; i-- ){
						new[i]= new[i-1];
					}
					new[cur]= ch;
					winsch(winComm, ch);
					new_len++;
				}
		}
		wrefresh(winComm);	// refresh the win
	}
	wclear(winComm);

	keypad(winComm,false);
	echo();
	cbreak();
	nl();
	new[new_len]= '\0';	// ensure the end syntax in new string
	return true;
}
