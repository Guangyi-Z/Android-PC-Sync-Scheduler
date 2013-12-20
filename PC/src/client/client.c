#include<sys/types.h>
#include<sys/socket.h>
#include<stdio.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>

int main(){
	int sockfd;
	int len;
	struct sockaddr_in addr;
	int res;
	
	sockfd= socket(AF_INET, SOCK_STREAM, 0);
	addr.sin_family= AF_INET;
	addr.sin_addr.s_addr= inet_addr("127.0.0.1");
	addr.sin_port= htons(9555);
	len= sizeof(addr);

	res= connect(sockfd, (struct sockaddr*)&addr, len);
	
	if( res == -1){
		perror("conn error: client");
	}
	//char buf[]="id:ID;title:TITLE;info:INFO;date:DATE;beginTime:BEGIN;endTime:END;syncFlag:4;";
	//char buf='A';
	char buf[1024];
	memset(buf, '\0',sizeof(buf));
	write(sockfd, buf, sizeof(buf) );

	res= 0;
	while( 1 ){
		res = read(sockfd, buf, sizeof(buf));
		if( buf[0] == '\0' )
			break;
		printf("%d: %s\n", res, buf);
	}
	close(sockfd);
	exit(0);

}
