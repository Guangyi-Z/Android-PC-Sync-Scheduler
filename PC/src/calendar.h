#ifndef CALENDAR
#define CALENDAR
#include<stdbool.h>

bool isValidMonth(int, int);
bool isValidDay(int y, int m, int d);
void goPrevDay(int* y, int* m, int *d);
void goNextDay(int* y, int* m, int *d);
bool updateCalendar(int, int, int);
int getCalendarArray(int **);

#endif
