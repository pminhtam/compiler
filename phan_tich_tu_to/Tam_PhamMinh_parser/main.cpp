#include<iostream>
#include<fstream>
#include<string>
#include<vector>
//#include "scanning.h"
#include "parser.h"


using namespace std;


int main(int argc, char **args){
	if (argc <= 1) 
		return -1;	
	
	get_tokens(args[1]);
	tokens_size = tokens.size();

	Token = get_token_parse();

	program();


}

