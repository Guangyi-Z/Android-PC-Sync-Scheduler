package com.example.scheduler;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import android.content.Context;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteDatabase.CursorFactory;
import android.database.sqlite.SQLiteOpenHelper;
import android.util.Log;

public class DatabaseHelper extends SQLiteOpenHelper{

	Context context= null;
	SQLiteDatabase db= null;
	private static String DATABASE_NAME= null;
	private static String TABLE_NAME= "scheduler";
	
	// tags for debug
    private static final String TAG_DB= "TEST_DB";
	
	public DatabaseHelper(Context context, String name, CursorFactory factory,
			int version) {
		super(context, name, factory, version);
		// TODO Auto-generated constructor stub
		this.context= context;
		DATABASE_NAME= name;
		// if database not found ,turn to onCreate() method
		db= getWritableDatabase(); 
	}

	@Override
	public void onCreate(SQLiteDatabase db) {
		// TODO Auto-generated method stub
		String sql= "CREATE TABLE " + TABLE_NAME + " (" +  
                "id TEXT PRIMARY KEY,"  
                + "title TEXT,"
                + "info TEXT,"
                + "date TEXT,"
                + "beginTime TEXT,"
                + "endTime TEXT,"
                + "syncFlag INTEGER"
                + ");";
		try {  
            db.execSQL(sql);  
            Log.v(TAG_DB, "Create Table ok: " + sql);  
        } catch (Exception e) {  
            Log.v(TAG_DB, "Create Table err, table exists: " + sql);  
        }
	}

	@Override
	public void onUpgrade(SQLiteDatabase db, int oldVersion, int newVersion) {
		// TODO Auto-generated method stub
		
	}
	
	public void insertNewEntry(ScheduleEntry _e){
		String sql="INSERT INTO " + TABLE_NAME + " VALUES ('"
				+ _e.getId() + "' ,'"
				+ _e.getTitle() + "' ,'"
				+ _e.getInfo() + "' ,'"
				+ _e.getDate() + "' ,'"
				+ _e.getBeginTime() + "' ,'"
				+ _e.getEndTime() + "' ,"
				+ _e.getSyncFlag()
				+ ");";  
		try {
            db.execSQL(sql);  
            Log.v(TAG_DB,"insert Table ok: " + sql);  
		} catch (Exception e){  
            Log.v(TAG_DB,"insert Table err ,sql: "+sql);  
        }  
        Log.v(TAG_DB, "" + db.getPath());  
	}
	
	public void deleteEntry(String id){
		String sql="DELETE FROM " + TABLE_NAME +
				" WHERE " +
				"id='" + id+ "';";  
		try {
            db.execSQL(sql);  
            Log.v(TAG_DB,"delete Table ok: " + sql);  
		} catch (Exception e){  
            Log.v(TAG_DB,"delete Table err ,sql: "+sql);  
        }  
		
	}
	
	public void markDeleteEntry(String id) {
		String sql="UPDATE " + TABLE_NAME + " SET syncFlag = " +
				"( CASE syncFlag" +
				" WHEN 0 THEN 4" +
				" WHEN 2 THEN 6" +
				" ELSE syncFlag END)" +
				" WHERE" +
				" id='" + id+ "';";  
		try {
            db.execSQL(sql);  
            Log.v(TAG_DB,"delete Table ok: " + sql);  
		} catch (Exception e){  
            Log.v(TAG_DB,"delete Table err ,sql: "+sql);  
        }  
		
	}
	
	public List<Map<String, Object>> queryEntries(int _y, int _m, int _d){
		List<Map<String, Object>> l= new ArrayList<Map<String, Object>>();
		
		String d= "" + _y + "-" + _m + "-" + _d;
		String sql= "SELECT id,"  
                    + "title,"
                    + "info,"
                    + "date,"
                    + "beginTime,"
                    + "endTime,"
                    + "syncFlag" +
                    " FROM " + TABLE_NAME + 
                    " WHERE " + TABLE_NAME + ".date = '" + d + "' " +
                    		"AND syncFlag != " + ScheduleEntry.FLAG_DELETED + 
                    		" AND syncFlag != " + (ScheduleEntry.FLAG_MODIFIED+ScheduleEntry.FLAG_DELETED) +
                    		";";
	    // entry data vars
	    String id,
		   title,
		   info,
		   beginTime,
		   endTime,
		   date;
	    int syncFlag;
	    // iterate the cursor
	    Cursor c= db.rawQuery(sql, null); 
	    c.moveToFirst(); 
	    while (! c.isAfterLast()) { 
	    	id= c.getString(0);
	        title= c.getString(1); 
	        info= c.getString(2);
	        beginTime= c.getString(4);
	        endTime= c.getString(5);
	        syncFlag= c.getInt(6);
	        // each map in the list stand for one node in ListView
	        Map<String, Object> m= new HashMap<String, Object>();
	        m.put("id_schedule", id);
	        m.put("title", title);
	        m.put("info", info);
	        m.put("time", beginTime+" - "+endTime);
	        m.put("sync", syncFlag);
	        
	        l.add(m);
	        
	        c.moveToNext(); 
	    } 
	    c.close();
	    return l;
	}

	public void updateEntry(ScheduleEntry _e) {
		// TODO Auto-generated method stub
		String sql="UPDATE " + TABLE_NAME + " SET "
				+ "title='" + _e.getTitle() + "' ,"
				+ "info='" + _e.getInfo() + "' ,"
				+ "beginTime='" + _e.getBeginTime() + "' ,"
				+ "date='" + _e.getDate() + "' ,"
				+ "endTime='" + _e.getEndTime() + "' ,"
				+ "syncFlag='" + _e.getSyncFlag() + "' "
				+ "WHERE id='"+ _e.getId() + "';";  
		try {
            db.execSQL(sql);  
            Log.v(TAG_DB,"update Table ok: " + sql);  
		} catch (Exception e){  
            Log.v(TAG_DB,"update Table err ,sql: "+sql);  
        }  
	}

	public List<Map<String, Object>> querySyncEntries(){
		List<Map<String, Object>> l= new ArrayList<Map<String, Object>>();
		
		String sql= "SELECT id,"  
                    + "title,"
                    + "info,"
                    + "date,"
                    + "beginTime,"
                    + "endTime,"
                    + "syncFlag" +
                    " FROM " + TABLE_NAME + 
                    " WHERE " + TABLE_NAME + ".syncFlag != 0 ;";
	    // entry data vars
	    String id,
		   title,
		   info,
		   beginTime,
		   endTime,
		   date;
	    int syncFlag;
	    // iterate the cursor
	    Cursor c= db.rawQuery(sql, null); 
	    c.moveToFirst(); 
	    while (! c.isAfterLast()) { 
	    	id= c.getString(0);
	        title= c.getString(1); 
	        info= c.getString(2);
	        date= c.getString(3);
	        beginTime= c.getString(4);
	        endTime= c.getString(5);
	        syncFlag= c.getInt(6);
	        // each map in the list stand for one node in ListView
	        Map<String, Object> m= new HashMap<String, Object>();
	        m.put("id", id);
	        m.put("title", title);
	        m.put("info", info);
	        m.put("date", date);
	        m.put("beginTime", beginTime);
	        m.put("endTime", endTime);
	        m.put("syncFlag", syncFlag);
	        
	        l.add(m);
	        
	        c.moveToNext(); 
	    } 
	    c.close();
	    return l;
	}

	public void deleteAllEntries() {
		String sql="DELETE FROM " + TABLE_NAME + ";";
		try {
            db.execSQL(sql);  
            Log.v(TAG_DB,"delete Table ok: " + sql);  
		} catch (Exception e){  
            Log.v(TAG_DB,"delete Table err ,sql: "+sql);  
        }  
		
	}
	
}
