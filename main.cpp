#include<iostream>
#include<fstream>
#include<string>
#include<vector>
//#include "scanning.h"
#include "scope.h"
#include "parser2.h"
#include "may_ao.h"


using namespace std;


int main(int argc, char **args){
	if (argc <= 1) 
		return -1;	
	
	get_tokens(args[1]);
	tokens_size = tokens.size();

	Token = get_token_parse();

	program();

show_instruction(1000);

cout<<"\n ==============================================="<<endl;
cout<<"Ket qua chay tren may ao "<<endl;
intepreter(Code);

}

