#ifndef CALENDAR
#define CALENDAR

#include<stdbool.h>
#include<time.h>

#define DAYS_MAX_LEN 49

int aDays[DAYS_MAX_LEN];
int aDays_len= 0;

int getDayLenOfMonth(int y, int m);

bool isValidDay(int y, int m, int d){
	struct tm date;
	date.tm_year= y+100;
	date.tm_mon= m-1;
	date.tm_mday= d;
	date.tm_hour= 0;
	date.tm_min= 0;
	date.tm_sec= 0;
	int res= mktime( &date );

	if( -1 == res || 
		date.tm_year != y+100 ||
		date.tm_mon != m-1 ||
		date.tm_mday == d ) {
		return false;
	} else {
		return true;
	}
}
bool isValidMonth(int y, int m){
	struct tm date;
	date.tm_year= y+100;
	date.tm_mon= m-1;
	date.tm_mday= 1;
	date.tm_hour= 0;
	date.tm_min= 0;
	date.tm_sec= 0;
	int res= mktime( &date );

	if( -1 == res || 
		date.tm_year != y+100 ||
		date.tm_mon != m-1 ||
		date.tm_mday == 0 ) {
		return false;
	} else {
		return true;
	}
}

void goPrevDay(int* y, int* m, int *d){
	if( *d == 1 ){	// back prev month
		if( *m != 1 ){
			(*m)--;
			*d= getDayLenOfMonth(*y, *m);
		} else {	// back prev year
			*m= 12;
			(*y)--;
			*d= 31;
		}
	} else {	// still in current month
		(*d)--;
	}
}

void goNextDay(int* y, int* m, int *d){
	int len= getDayLenOfMonth(*y, *m);
	if( *d == len ){	// go next month
		if( *m != 12 ){
			(*m)++;
			*d= 1;
		} else {	// go next year
			*m= 1;
			(*y)++;
			*d= 1;
		}
	} else {	// still in current month
		(*d)++;
	}
}

/*
** update the days array according to the date supplied, and draw the calendar on screen without refresh
** return false if date is not valid
*/
bool updateCalendar(int y, int m, int d){
	// get number of days in current|prev month
	int aDays_len_thisMonth= getDayLenOfMonth(y, m);
	int aDays_len_lastMonth= -1;
	if( m != 1 )
		aDays_len_lastMonth= getDayLenOfMonth(y, m-1);
	else aDays_len_lastMonth= getDayLenOfMonth(y-1, 12);
	// specify the weekday of the first day of current month
	struct tm date;
	date.tm_year= y+100;
	date.tm_mon= m-1;
	date.tm_mday= 1;
	date.tm_hour= 0;
	date.tm_min= 0;
	date.tm_sec= 0;
	int res= mktime( &date );
	if( -1 == res || 	// valid date test
		date.tm_year != y+100 ||
		date.tm_mon != m-1 ||
		date.tm_mday != date.tm_mday ) {
		return false;
	}
	int wday_first= date.tm_wday;
	if( wday_first == 0 )
		wday_first= 7;
	// fill the aDays array
	// fill current month
	int index= wday_first-1;
	for(int i= 0; i< aDays_len_thisMonth; i++){
		aDays[index++]= i+1;
	}
	// fill prev month
	for(int i=wday_first-2; i>=0; i--){
		aDays[i]= aDays_len_lastMonth--;
	}
	// fill next month
	int n= wday_first-1 + aDays_len_thisMonth;
	if( n <=35 )
		aDays_len= 35;
	else if( n <= 42 )
		aDays_len= 42;
	else aDays_len= 49;
	int aDays_len_nextMonth= aDays_len - n;
	for( int i= 0; i<aDays_len_nextMonth; i++ ){
		aDays[aDays_len - aDays_len_nextMonth + i]= i + 1;
	}
	return false;
}

bool isLeapYear(int y){
	if( y%100 == 0 && y%400 == 0 ||
		y%100 != 0 && y%4 == 0 )
		return true;
	return false;
}

int getDayLenOfMonth(int y, int m){
	switch(m){
		case 2:
			if( isLeapYear(y) )
				return 29;
			else return 28;
		case 1:
		case 3:
		case 5:
		case 7:
		case 8:
		case 10:
		case 12:
			return 31;
		default:
			return 30;
	}
}

/*
** return the length array of calendar
*/
int getCalendarArray(int** ap){
	*ap= aDays;
	return aDays_len;
}
#endif




