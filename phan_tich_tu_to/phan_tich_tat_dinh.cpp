#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include <stdlib.h>

using namespace std;

void S();
void A();
void B();
void C();
void D();

char Ch;
int i_ch = 0;
string s;
int sx[100];
int n=0;
void nextCh(){
	Ch =  s[i_ch++];
}
void error(string s){
	cout<<"\n Loi:  "<<s<<endl;
//	exit(EXIT_SUCCESS);
}

void S(){
	sx[n++]=1;

	B();
	A();
	

}
void A(){
    if( Ch=='+') {
    	sx[n++]=2;
		nextCh();
	  	B();
		A();
	}
	else{
		sx[n++]=3;
	}
}
void B(){
	sx[n++]=4;
	D();
	C();
}
void C(){
	if( Ch=='*') {
		sx[n++]=5;
		nextCh();
	  	D();
		C();
	}
	else{
		sx[n++]=6;
	}
}
void D(){
	if(Ch=='('){
		sx[n++]=7;
		nextCh();
		S();
		if(Ch==')') nextCh();
		else error("Thieu dong ngoac");
	} 
	else if(Ch=='a'){
		sx[n++]=8;
		nextCh();
	}
	
	else error("Thieu toan hang");
}	

int main(){
	do{
//		s = "";
		i_ch = 0;
		cin>>s;
		nextCh();
		S();
		printf("Bieu thuc hop le...\n");
		
		for(int i=0;i<n;i++){
			cout<<sx[i]<<" ";
		}
		cout<<endl;
		n=0;
	}while(1);
	
}

