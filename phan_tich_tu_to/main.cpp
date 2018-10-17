#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include "scanning.h"


using namespace std;


int main(int argc, char **args){
	if (argc <= 1) 
		return -1;	
	ifstream sfile;
	sfile.open(args[1],ios_base::in);
	sfile.unsetf(ios_base::skipws);	
	

	TokenType token;
	do{
		token = getToken(sfile);
		tokens.push_back(token);
		
	}while(token!=NONE);
	
	show_token();


}

