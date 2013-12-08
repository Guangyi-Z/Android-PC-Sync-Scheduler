package com.example.scheduler;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.graphics.Color;
import android.graphics.drawable.ColorDrawable;
import android.os.Bundle;
import android.util.Log;
import android.view.Display;
import android.view.Gravity;
import android.view.Menu;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.view.View.OnClickListener;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.Button;
import android.widget.GridView;
import android.widget.LinearLayout;
import android.widget.TextView;

public class CalendarActivity extends Activity {

	AdapterManager am= null;
	GridView gridView;
	TextView textTop= null;
	Button btnPre= null, 
			btnNext= null;
	
	public static final String SELECTED_DATE= "1"; 
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		requestWindowFeature(Window.FEATURE_NO_TITLE);
		setContentView(R.layout.activity_calendar);
		// get data from parent activity
		Intent intent= CalendarActivity.this.getIntent();
		Bundle map= intent.getExtras();
		String dateOfMain= (String)map.get(MainActivity.SCHEDULE_DATE);
		String[] aDate= dateOfMain.split("-"); 
		am= new AdapterManager(this, Integer.parseInt(aDate[0]), Integer.parseInt(aDate[1]));
		// the content of the calendar
		gridView= makeGridView(this);
		gridView.setAdapter(am.getAdapter());
		gridView.setOnItemClickListener(new OnItemClickListener(){

			@Override
			public void onItemClick(AdapterView<?> arg0, View arg1, int arg2,
					long arg3) {
				// TODO Auto-generated method stub
				String dateBack= am.getShowingDate() + "-" + am.getDayByPositon(arg2);
				Log.i("grid","item clicked: " + dateBack);
				// return to the Main Activity
				Intent intent= CalendarActivity.this.getIntent();
				
				Intent intentBack= new Intent();
				intentBack.putExtra(SELECTED_DATE, dateBack);
				CalendarActivity.this.setResult(RESULT_OK, intentBack);
				CalendarActivity.this.finish();
			}
			
		});
		LinearLayout cont = (LinearLayout) findViewById(R.id.calendar_content);
        cont.addView(gridView);
        
        // the text showing current month of calendar
 		textTop= (TextView)this.findViewById(R.id.toptext);
 		setTextTop();
		
		// previous & next button
		btnPre= (Button)this.findViewById(R.id.previous);
		btnPre.setOnClickListener(new OnClickListener(){

			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				am.toPreviousMonth();
				gridView.setAdapter(am.getAdapter());
				setTextTop();
			}
			
		});
		btnNext= (Button)this.findViewById(R.id.next);
		btnNext.setOnClickListener(new OnClickListener(){

			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				am.toNextMonth();
				gridView.setAdapter(am.getAdapter());
				setTextTop();
			}
			
		});
        
		// test button
		Button btnTest= (Button)this.findViewById(R.id.test);
		btnTest.setOnClickListener(new OnClickListener(){

			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				Intent intent= CalendarActivity.this.getIntent();
				
				Intent intentBack= new Intent();
				CalendarActivity.this.setResult(2, intentBack);
				CalendarActivity.this.finish();
			}
			
		});
		
		
	}

	private void setTextTop() {
		// TODO Auto-generated method stub
		String sd= am.getShowingDate();
 		StringBuffer sb= new StringBuffer();
 		sb.append(sd);
 		textTop.setText(sb);
	}

	private GridView makeGridView(Context _c) {
		// TODO Auto-generated method stub
		WindowManager windowManager = getWindowManager();
        Display display = windowManager.getDefaultDisplay();
        int Width = display.getWidth(); 
        int Height = display.getHeight();
		
		GridView gv= new GridView(_c);
		gv.setNumColumns(7);
		gv.setColumnWidth(46);
		if(Width == 480 && Height == 800){
			gv.setColumnWidth(69);
		}
		gv.setGravity(Gravity.CENTER_VERTICAL);
		//gv.setSelector(new ColorDrawable(Color.GRAY)); // 去除gv边框
		gv.setVerticalSpacing(1);
		gv.setHorizontalSpacing(1);
		gv.setBackgroundColor(Color.RED);
        //gv.setBackgroundResource(R.color.light_gray_bg);
        gv.setPadding(1, 1, 1, 1);
		return gv;
	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}
	
	protected void onActivityResult(){
		
	}

}
