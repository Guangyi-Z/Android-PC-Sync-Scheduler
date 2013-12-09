package com.example.scheduler;

import java.util.Date;
import java.text.ParseException;
import java.text.SimpleDateFormat;

import android.util.Log;

/**
 * Schedule entry, a java bean class
 * @author janfan
 *
 */
public class ScheduleEntry {
	
	public static final int FLAG_UNCHANGED= 0,
							 FLAG_NEW= 1,
							 FLAG_MODIFIED= 2,
							 FLAG_DELETED= 4;
	SimpleDateFormat sdf= new SimpleDateFormat("yyyy-MM-dd");
	
	private String id,
				   title,
				   info,
				   beginTime,
				   endTime,
				   date;
	private int syncFlag;
	
	private static final String TAG_ENTRY= "TEST_ENTRY";
	
	public ScheduleEntry(){ };
	public ScheduleEntry(String _id, 
						 String _title, 
						 String _info, 
						 String _date, 
						 String _begTime, 
						 String _endTime, 
						 int _syncFlag){
		id= _id;
		title= _title;
		info = _info;
		date= _date;
		Log.v(TAG_ENTRY, date);
		beginTime= _begTime;
		endTime= _endTime;
		syncFlag= _syncFlag;
	}
	
	/*
	 * Getters & Setters
	 */
	public String getId() {
		return id;
	}
	public void setId(String id) {
		this.id = id;
	}
	public String getTitle() {
		return title;
	}
	public void setTitle(String title) {
		this.title = title;
	}
	public String getInfo() {
		return info;
	}
	public void setInfo(String info) {
		this.info = info;
	}
	public String getBeginTime() {
		return beginTime;
	}
	public void setBeginTime(String beginTime) {
		this.beginTime = beginTime;
	}
	public String getEndTime() {
		return endTime;
	}
	public void setEndTime(String endTime) {
		this.endTime = endTime;
	}
	public String getDate() {
		return date;
	}
	public void setDate(String date) {
		this.date = date;
	}
	public int getSyncFlag() {
		return syncFlag;
	}
	public void setSyncFlag(int syncFlag) {
		this.syncFlag = syncFlag;
	}
}
