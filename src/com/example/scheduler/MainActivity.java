package com.example.scheduler;

import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import android.os.Bundle;
import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.database.sqlite.SQLiteDatabase;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.SimpleAdapter;

public class MainActivity extends Activity {

	private Button btnCal= null,
					btnNew= null;
	private ListView lTask= null;
	// the date whose schedules are shown
	private int year,
		month,
		day;
	// activities invoke intent flags
	public static final String SCHEDULE_DATE="1";
	
	private static final String DATABASE_NAME = "scheduler.db";
	DatabaseHelper dbh= null;
	
	// tags for debug
	private static final String TAG_DIALOG= "TEST_DIALOG";
    
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		// set up the system current time
		Date date = new Date();
		SimpleDateFormat sdf = new SimpleDateFormat("yyyy-M-d");
		String sysDate = sdf.format(date);
		year = Integer.parseInt( sysDate.split("-")[0] );
		month = Integer.parseInt( sysDate.split("-")[1] );
		day = Integer.parseInt( sysDate.split("-")[2] );
		Log.i("TEST","MAIN AC INIT:"+year+"-"+month+"-"+day);
		
		// set up datebase
		dbh= new DatabaseHelper(this, DATABASE_NAME, null, 1);  
		
		// set up widgets
		btnCal= (Button)this.findViewById(R.id.go_calendar);
		btnNew= (Button)this.findViewById(R.id.new_entry);
		lTask= (ListView)this.findViewById(R.id.task_listView);
		
		btnCal.setOnClickListener(new OnClickListener(){

			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
			    Intent intent = new Intent(MainActivity.this, CalendarActivity.class); 
			    intent.putExtra(SCHEDULE_DATE, ""+year+"-"+month+"-"+day);
			    Log.i("TEST","MAIN AC LISTENER:"+MainActivity.this.year+"-"+MainActivity.this.month);
			    MainActivity.this.startActivityForResult(intent, 1);  
			}
			
		});
		// for NEW ENTRY button
		btnNew.setOnClickListener(new OnClickListener(){

			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				LayoutInflater inflater = getLayoutInflater();
				
				final View layout = inflater.inflate(R.layout.new_entry_dialog,
						(ViewGroup)findViewById(R.id.dialog_title));
				AlertDialog.Builder builder= new AlertDialog.Builder(MainActivity.this)
						.setTitle("New Entry")
						.setView(layout);
				builder.setPositiveButton("Add", new DialogInterface.OnClickListener(){

					@Override
					public void onClick(DialogInterface arg0, int arg1) {
						// TODO Auto-generated method stub
						ScheduleEntry en= new ScheduleEntry();
						en.setId("ID_"+(int)(Math.random()*100000)); 
						en.setDate("" + year + "-" + month + "-" + day);
						en.setSyncFlag(ScheduleEntry.FLAG_NEW);
						// get value entered by user
						en.setTitle(getValue(R.id.dialog_title_et));
						en.setInfo(getValue(R.id.dialog_info_et));
						en.setBeginTime(getValue(R.id.dialog_beg_et));
						en.setEndTime(getValue(R.id.dialog_end_et));
						// insert the new entry
						dbh.insertNewEntry(en);
						MainActivity.this.updateListView(year, month, day);
					}
					
					private String getValue(int _rid){
						EditText et = (EditText) layout.findViewById(_rid);
			            if(et == null) 
			            	Log.d(TAG_DIALOG, "NULL");
			            else{
			                String value = et.getText().toString();
			                Log.v(TAG_DIALOG, value);
			                return value;
			            }
			            return null;
					}
					
				});
				builder.setNegativeButton("Cancel", null);
				builder.create().show();
			}
			
		});
		// update the ListView
		updateListView(year, month, day);
	}

	/**
	 * get schedule entries of the specific day that is pointed by params from db
	 * and show them on ListView
	 * @param year2
	 * @param month2
	 * @param day2
	 */
	private void updateListView(int year2, int month2, int day2) {
		// TODO Auto-generated method stub
		// try get data from sqlite3
		
		// bind the data with adapter
		SimpleAdapter adapter = new SimpleAdapter(this, dbh.queryEntries(year2, month2, day2), 
				R.layout.schedule_item,
				new String[]{"title","info","beginTime"},
				new int[]{R.id.title,R.id.info,R.id.time});
		lTask.setAdapter(adapter);
	}
	
	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}
	
	/**
	 * return from Calendar activity, get selected day from calendar 
	 * and update the year/month/day of showing schedule
	 */
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		if(resultCode == RESULT_OK){
			Bundle map= data.getExtras();
			String date= (String)map.get(CalendarActivity.SELECTED_DATE);
			String[] aDate= date.split("-"); 
			year= Integer.parseInt(aDate[0]);
			month= Integer.parseInt(aDate[1]);
			day= Integer.parseInt(aDate[2]);
			Log.i("TEST","MAIN AC BACK:"+year+"-"+month+"-"+day);
			updateListView(year, month, day);
			// update the ListView
		} else {
			Log.e("ac-invoke", "RESULT BACK TO MAIN AC");
		}
	}

}
