package com.example.scheduler;

import java.text.SimpleDateFormat;
import java.util.Date;

import android.content.Context;
import android.util.Log;
import android.widget.ListAdapter;

public class AdapterManager {
	
	CalAdapter ca= null;
	Context context;
	
	// general time settings
	private int dayOfWeek = 0;        // 0 for Monday, 6 for Sunday
	
	//current showing time
	private int currentYear = -1;
	private int currentMonth = -1;
	
	AdapterManager (Context _c, int _y, int _m){
		context= _c;
		// get system current date
		Date date = new Date();
		SimpleDateFormat sdf = new SimpleDateFormat("yyyy-M-d");
		String sysDate = sdf.format(date);
		int sys_year = Integer.parseInt( sysDate.split("-")[0] );
		int sys_month = Integer.parseInt( sysDate.split("-")[1] );
		int sys_day = Integer.parseInt( sysDate.split("-")[2] );
		// set current showing date
		currentYear= _y;
		currentMonth= _m;
		// initial the adapter according to system date
		Log.i("TEST","ADAPTER INIT:"+currentYear+"-"+currentMonth);
		ca = new CalAdapter(context, currentYear, currentMonth);
		
	}

	public void toPreviousMonth() {
		// TODO Auto-generated method stub
		// update the current showing date
		if(currentMonth == 1){
			currentYear--; 
			currentMonth= 12;
		} else 
			currentMonth--;
		updateAdapter();
		
	}

	public void toNextMonth() {
		// TODO Auto-generated method stub
		// update the current showing date
		if(currentMonth == 12){
			currentYear++; 
			currentMonth= 1;
		} else 
			currentMonth++;
		updateAdapter();
	}

	/**
	 * return the showing year&month in "yyyy-m" format String
	 * @return
	 */
	public String getShowingDate() {
		// TODO Auto-generated method stub
		return "" + currentYear + "-" + currentMonth;
	}
	
	/**
	 * update the adapter according to current showing date
	 */
	private void updateAdapter(){
		ca= new CalAdapter(context, currentYear, currentMonth);
	}
	
	/**
	 * get current Calendar GridView adapter
	 * @return
	 */
	public ListAdapter getAdapter() {
		// TODO Auto-generated method stub
		return ca;
	}

	/**
	 * return the day by taking the position as index  
	 * @param arg2 position
	 * @return
	 */
	public int getDayByPositon(int arg2) {
		// TODO Auto-generated method stub
		return ca.getDayByPosition(arg2);
	}
}
