package com.example.scheduler;

import java.text.SimpleDateFormat;
import java.util.Date;

import com.example.util.CalendarUtil;

import android.content.Context;
import android.util.Log;
import android.view.View;
import android.view.ViewGroup;
import android.view.ViewGroup.LayoutParams;
import android.widget.BaseAdapter;
import android.widget.GridView;
import android.widget.TextView;

public class CalAdapter extends BaseAdapter {

	private String[] lCalDays= new String[49];	// store all the days of current showing grid view
	Context context;
	
	// general time vars
	private int daysOfLastMonth = -1;
	private int daysOfMonth= -1;
	private int indexOfFirstDayOfMonth= -1;
	
	private int sys_year = -1;
	private int sys_month = -1;
	private int sys_day = -1;
	int indexOfSysDay;
	//current showing time
	private int currentYear = -1;
	private int currentMonth = -1;
	
	CalAdapter (Context _c, int currentYear2, int currentMonth2){
		context= _c;
		
		// get the current time of system
		Date date = new Date();
		SimpleDateFormat sdf = new SimpleDateFormat("yyyy-M-d");
		String sysDate = sdf.format(date);
		sys_year = Integer.parseInt( sysDate.split("-")[0] );
		sys_month = Integer.parseInt( sysDate.split("-")[1] );
		sys_day = Integer.parseInt( sysDate.split("-")[2] );
		// set the Adapter data according to params
		updateShowingDate(currentYear2, currentMonth2);
	}

	/**
	 * update the current showing date & other time vars & calendar days list
	 * @param _y
	 * @param _m
	 */
	public void updateShowingDate(int _y, int _m) {
		// TODO Auto-generated method stub
		// set the time vars according to sys date
		daysOfMonth= CalendarUtil.getDaysOfMonth( _y,  _m);
		if( _m == 1 )
			daysOfLastMonth= CalendarUtil.getDaysOfMonth( _y-1, 12);
		else daysOfLastMonth= CalendarUtil.getDaysOfMonth( _y,  _m-1);
		indexOfFirstDayOfMonth= CalendarUtil.getWeekdayOfFirstDayOfMonth( _y,  _m);
		// set the current date same as sys date
		currentYear= _y;
		currentMonth= _m;
		// update the calendar days list
		fillCalDaysList(_y, _m);
	}
	
	public void fillCalDaysList(int year, int month){
		for (int i = 0; i < lCalDays.length; i++) {
			if(i < indexOfFirstDayOfMonth){	// for last month
				lCalDays[i] = (daysOfLastMonth - indexOfFirstDayOfMonth + 1 + i) + ".";
			} 
			else if(i < daysOfMonth + indexOfFirstDayOfMonth){   // for current showing month
				int day = i - indexOfFirstDayOfMonth + 1;
				lCalDays[i] = (i - indexOfFirstDayOfMonth + 1) + ".";
				// Mark the sys current date
				if(sys_year == year
						&& sys_month == month 
						&& sys_day == day ){
					indexOfSysDay = i;
				}
			} 
			else{   // for next month
				lCalDays[i] = (i - daysOfMonth - indexOfFirstDayOfMonth + 1) + ".";
			}
		}
	}
	
	/**
	 * get the position of the first day of current showing month
	 * @return
	 */
	public int getStartPositon(){
		return indexOfFirstDayOfMonth;
	}
	
	/**
	 * get the position of the last day of current showing month
	 * @return
	 */
	public int getEndPosition(){
		return  (indexOfFirstDayOfMonth+daysOfMonth)-1;
	}

	@Override
	public int getCount() {
		// TODO Auto-generated method stub
		if(currentMonth == 2)
		{
			if(getStartPositon() == 0){
				if(! CalendarUtil.isLeapYear(currentYear))
					return 28;	// 4 rows
			} else	
				return 35;	// 5 rows
		}
		// if the first day of current month is positioned behind index 4, 6 rows are needed
		return getStartPositon() <= 4 ? 35 : 42;	
	}

	@Override
	public Object getItem(int position) {
		// TODO Auto-generated method stub
		return position;
	}

	@Override
	public long getItemId(int position) {
		// TODO Auto-generated method stub
		return position;
	}
	
	@Override
	public View getView(int position, View convertView, ViewGroup parent) {
		// TODO Auto-generated method stub
		TextView tv= null;
		if(convertView == null){
			tv= new TextView(context);
		} else {
			tv= (TextView)convertView;
		}
		GridView.LayoutParams p = new GridView.LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT);
		tv.setLayoutParams(p);
		tv.setText(lCalDays[position]);
		
		return tv;
	}

	/**
	 * return the day by taking the position as index  
	 * @param arg2 position
	 * @return
	 */
	public int getDayByPosition(int arg2) {
		// TODO Auto-generated method stub
		return Integer.parseInt( lCalDays[arg2].substring(0, lCalDays[arg2].length()-1) );
	}
}
