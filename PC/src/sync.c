#include<sys/types.h>
#include<sys/socket.h>
#include<stdio.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<stdlib.h>
#include"entry.h"
#include"db_entry.h"
#include<string.h>
#include<time.h>

#define PORT 9555
#define BUF_SIZE 1024
#define ENTRY_SIZE 1024

char buf[BUF_SIZE];
int sockServer, sockClient;

int count2semicolon(char *p);

void sync_parseEntry(char * buf);
bool sync_entry(struct Entry * e);
void sync_PCEntry();

void sync_init(){
	int lenServer, lenClient;
	struct sockaddr_in addrServer;
	struct sockaddr_in addrClient;
	
	// create a socket for server
	sockServer= socket(AF_INET, SOCK_STREAM, 0);

	// bind the socket with localhost PORT
	addrServer.sin_family= AF_INET;
	addrServer.sin_addr.s_addr= htonl(INADDR_ANY);
	addrServer.sin_port= htons(PORT);
	lenServer= sizeof(addrServer);
	bind(sockServer, (struct sockaddr*)&addrServer, lenServer);

	// listen
	listen(sockServer, 5);
	lenClient= sizeof(addrClient);
	sockClient= accept(sockServer, (struct sockaddr* )&addrClient, &lenClient);

	// read from client
	char success[]= "OK\0";
	while(1){
		int res= read(sockClient, buf, sizeof(buf));
		if( buf[0] == '\0' )
			break;
		sync_parseEntry(buf);
		write(sockClient, success, sizeof(success));
	}

	// sync the entries of PC itself, clear the flags of new && modified entries, del all deleted entries
	sync_PCEntry();

	// send updated entries to mobile
	struct Entry l[ENTRY_SIZE];

	int counter= 0;
	int len_entries= db_getAllEntries(l);
	while( counter < len_entries ){
		memset(buf, '\0', BUF_SIZE);
		strcat(buf, "id:");
		strcat(buf, l[counter].id);
		strcat(buf, ";");
		strcat(buf, "title:");
		strcat(buf, l[counter].title);
		strcat(buf, ";");
		strcat(buf, "info:");
		strcat(buf, l[counter].info);
		strcat(buf, ";");
		strcat(buf, "date:");
		strcat(buf, l[counter].date);
		strcat(buf, ";");
		strcat(buf, "beginTime:");
		strcat(buf, l[counter].beginTime);
		strcat(buf, ";");
		strcat(buf, "endTime:");
		strcat(buf, l[counter].endTime);
		strcat(buf, ";");
		strcat(buf, "syncFlag:");
		char flag[2];
		sprintf(flag, "%d", l[counter].syncFlag);
		flag[1]= '\0';
		strcat(buf, flag);
		strcat(buf, ";");

		counter++;
		write(sockClient, buf, sizeof(buf));
		char response[5];
		read(sockClient, response, sizeof(response));
		int resCmp= strcmp(response, "OK\0");
	}
	// send a empty buf for ending
	memset(buf, '\0', BUF_SIZE);
	write(sockClient, buf, sizeof(buf));

		/*
		char* path= "sync_log.txt";
		char* mode= "w+";
		flog= fopen(path, mode);
		fprintf(flog, "%s\n%s\n%s\n%s\n%s\n%s\n%d\n",
			e.id,
			e.title,
			e.info,
			e.date,
			e.beginTime,
			e.endTime,
			e.syncFlag);
		fclose(flog);
		*/
}

void sync_parseEntry(char * buf){
		//parse the buf
		char *p= NULL;
		struct Entry e;
		int count= 0;
		p= strstr(buf, "id:");
		p= p+3;
		count= count2semicolon(p);
		memcpy(e.id, p, count);
		e.id[count]= '\0';
		p= p+count;

		p= strstr(buf, "title:");
		p= p+6;
		count= count2semicolon(p);
		memcpy(e.title, p, count);
		e.title[count]= '\0';
		p= p+count;

		p= strstr(buf, "info:");
		p= p+5;
		count= count2semicolon(p);
		memcpy(e.info, p, count);
		e.info[count]= '\0';
		p= p+count;

		p= strstr(buf, "date:");
		p= p+5;
		count= count2semicolon(p);
		memcpy(e.date, p, count);
		e.date[count]= '\0';
		p= p+count;

		p= strstr(buf, "beginTime:");
		p= p+10;
		count= count2semicolon(p);
		memcpy(e.beginTime, p, count);
		e.beginTime[count]= '\0';
		p= p+count;

		p= strstr(buf, "endTime:");
		p= p+8;
		count= count2semicolon(p);
		memcpy(e.endTime, p, count);
		e.endTime[count]= '\0';
		p= p+count;

		p= strstr(buf, "syncFlag:");
		p= p+9;
		count= count2semicolon(p);
		e.syncFlag= *p - '0';

		//sync a single entry
		sync_entry(&e);
}

bool sync_entry(struct Entry * e){
	struct Entry ePC;
	// for a new entry
	if( e->syncFlag % 2 == 1){
		e->syncFlag= 0;
		db_addEntry(*e);
	}
	// for a deleted but modified entry
	else if( e->syncFlag >= 4){
		if( false == db_getEntry(&ePC, e->id) )
			return false;
		if( ePC.syncFlag != 2 ){	// only entry modified in PC will be kept
			db_delEntry(e->id);
		}
		ePC.syncFlag= 0;
		db_modEntry(&ePC);
	}
	// for a modified entry
	else if( e->syncFlag == 2 ){
		if( false == db_getEntry(&ePC, e->id) )
			return false;
		if( ePC.syncFlag == 0){	// overwrite the entry in PC
			e->syncFlag= 0;
			db_modEntry(e);
		} else {	// add entry as a dup entry
			ePC.syncFlag= 0;
			db_modEntry(&ePC);
			// add the dup entry
			e->syncFlag= 0;
			srand(time(NULL));
			sprintf(e->id, "ID_%d", (rand() % 100000) );	// id range(0,100000)
			db_addEntry(*e);
		}
	}
	return true;
}

void sync_PCEntry(){
	db_syncEntries();
}

/*
** pass a char buf in, and return the count from pointer p to next ;
** return -1 if meet '\0'
*/
int count2semicolon(char *p){
	int count= 0;
	while(p){
		if( !p || *p == '\0')
			return -1;
		if( *p == ';')
			return count;
		p++;
		count++;
	}
	return -1;	// never been here
}

void sync_close(){
	close(sockServer);
}
