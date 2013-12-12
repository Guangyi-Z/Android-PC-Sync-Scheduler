#include<string.h>

int parseOneInt(char* str){
	int n= 0;
	// skip the char until blankspace
	while(str[n] != '\0'){
		if(str[n++] == ' '){
			break;
		}
	}
	int res= 0;
	while(str[n] != '\0'){
		res+= (str[n++] - '0');
		res*= 10;
	}
	res/= 10;
	if( 0 == res )
		return -1;
	else return res;
}
void parseTwoInt(char* str, int* n1, int *n2){
	int n= 0;
	// skip the char until blankspace
	while(str[n] != '\0'){
		if(str[n++] == ' '){	// only one blankspace is allowed
			break;
		}
	}
	if( str[n] == '\0' ){
		*n1= -1;
		*n2= -1;
		return;
	}
	// first arg
	int res= 0;
	while(str[n] != '\0' && str[n] != '-'){
		res+= (str[n++] - '0');
		res*= 10;
	}
	res/= 10;
	if( 0 == res )
		*n2= -1;
	else *n1= res;
	// second arg
	res= 0;
	if( str[n] == '-' ){
		n++;
		while(str[n] != '\0'){
			res+= (str[n++] - '0');
			res*= 10;
		}
	}
	res/= 10;
	if( 0 == res )
		*n2= -1;
	else *n2= res;
}

