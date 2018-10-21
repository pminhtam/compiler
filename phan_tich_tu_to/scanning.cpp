#include<iostream>
#include<fstream>
#include<string>
#include<vector>
//#include <ctype.h>


using namespace std;

typedef enum {
	NONE=0, IDENT, NUMBER,
	BEGIN, CALL, CONST, DO,  ELSE, END, FOR, IF, ODD,
	PROCEDURE, PROGRAM, THEN, TO, VAR, WHILE,
	PLUS, MINUS, TIMES, SLASH, EQU, NEQ, LSS, LEQ, GRT, GEQ, LPARENT, RPARENT, LBRACK, RBRACK, PERIOD, COMMA, SEMICOLON,  ASSIGN, PERCENT
} TokenType;

string key_words[] = {"BEGIN", "CALL", "CONST", "DO", "ELSE", "END", "FOR", "IF", "ODD",
					"PROCEDURE", "PROGRAM", "THEN", "TO", "VAR", "WHILE"};
					
string type_words[] = {"NONE", "IDENT", "NUMBER",
	"BEGIN", "CALL", "CONST", "DO",  "ELSE", "END", "FOR", "IF", "ODD",
	"PROCEDURE", "PROGRAM", "THEN", "TO", "VAR", "WHILE",
	"PLUS", "MINUS", "TIMES", "SLASH", "EQU", "NEQ", "LSS", "LEQ", "GRT", "GEQ", "LPARENT",
	 "RPARENT", "LBRACK", "RBRACK", "PERIOD", "COMMA", "SEMICOLON",  "ASSIGN", "PERCENT"};
int key_words_len = 15;
int line=0,column=0;
char ch = ' ';
int max_num = 9;

vector<TokenType> tokens;
vector<string> idents;
vector<int> numbers;

char getCh(ifstream& sfile){
	char c;
	if(!sfile.eof()){
		sfile>>c;
		column++;
		
		if(c == '\n'){
			line++;
			column++;
			sfile>>c;
		}
	}
	else{
		return 0;
	}
	char low_c =toupper(c);
//	cout<<low_c<<endl; 
	return low_c;
}
int check_letter(char ch){
	return isalpha(ch);
}

int check_digit(char ch){
	return isdigit(ch);
}

TokenType test_ident(string s){
	int i=0;
	for (i = 0;i<key_words_len;i++){
		if(s.compare(key_words[i]) == 0){			
			TokenType type = static_cast<TokenType>(i+3);;
			return type;
		}
	}
	
	idents.push_back(s);
//	cout<<s<<endl;
	return IDENT;
	
}

TokenType test_number(string s){
	int value = 0;
	int i= 0; 
	int L = s.length();
	
	if(L>max_num){
		cout<<"So qua lon"<<endl;
		return NONE;
	}
	
	if(check_digit(s[i])){ 
		value = value*10 + s[i]-48;	
		i=i+1;		
		while(check_digit(s[i])){
			value = value*10 + s[i]-48;
			i = i + 1; 
		}
			
			
		if(i==L) {
//			cout<<"so nguyen gia tri: "<<value<<endl;
			numbers.push_back(value);
			return NUMBER; 
		}
	}
	return NONE;
	
}

TokenType getToken(ifstream& sfile){
	while(ch == ' '){
		ch = getCh(sfile);
	}
	string s;
	if(check_letter(ch)){
		s+=ch;
		ch = getCh(sfile);
		while(check_letter(ch) || check_digit(ch)){
			s+=ch;
			ch = getCh(sfile);
		};
//		s+='\0';
		return test_ident(s);
	}
	
	else if(check_digit(ch)){
		s+=ch;
		ch = getCh(sfile);
		while(check_digit(ch)){
			s+=ch;
			ch = getCh(sfile);
		};
//		s+='\0';
		return test_number(s);
	}
	
	else if(ch == '<'){
		ch = getCh(sfile);
		if(ch == '>'){
			ch = getCh(sfile);
			return NEQ;
		}
		else if(ch == '='){
			ch = getCh(sfile);
			return LEQ;
		}
		else{
			return LSS;
		}
		
	}
	
	else if(ch == '>'){
		ch = getCh(sfile);
		if(ch == '='){
			ch = getCh(sfile);
			return GEQ;
		}
		else{
			return GRT;
		}
	}
	
	else if(ch == ':'){
		ch = getCh(sfile);
		if(ch == '='){
			ch = getCh(sfile);
			return ASSIGN;
		}
	}
	else if(ch == '+'){
		ch = getCh(sfile);
		return PLUS;
	}
	else if(ch == '-'){
		ch = getCh(sfile);
		return MINUS;
	}
	else if(ch == '*'){
		ch = getCh(sfile);
		return TIMES;
	}
	
	else if(ch == '/'){
		ch = getCh(sfile);
		return SLASH;
	}
	else if(ch == '%'){
		ch = getCh(sfile);
		return PERCENT;
	}
	
	else if(ch == '='){
		ch = getCh(sfile);
		return EQU;
	}
	
	else if(ch == '('){
		ch = getCh(sfile);
		return LPARENT;
	}
		
	else if(ch == ')'){
		ch = getCh(sfile);
		return RPARENT;
	}
	
	else if(ch == '['){
		ch = getCh(sfile);
		return LBRACK;
	}
	
	else if(ch == ']'){
		ch = getCh(sfile);
		return RBRACK;
	}
	
	else if(ch == '.'){
		ch = getCh(sfile);
		return PERIOD;
	}
	
	else if(ch == ','){
		ch = getCh(sfile);
		return COMMA;
	}
	
	else if(ch == ';'){
		ch = getCh(sfile);
		return SEMICOLON;
	}
	
	else{
		return NONE;
	}
}


void show_token(){
	int n_number = 0;
	int n_ident = 0;
	int l = tokens.size();
	for(int i=0;i<l;i++){
		
		
		if(tokens[i] == IDENT){
			cout<<type_words[(int) tokens[i]]<<"  (  "<<idents[n_ident]<<"  )"<<endl;
			n_ident++;
		}
		else if(tokens[i] == NUMBER){
			cout<<type_words[(int) tokens[i]]<<"  (  "<<numbers[n_number]<<"  )"<<endl;
			n_number++;
		}
		else{
			cout<<type_words[(int) tokens[i]]<<endl;
		}
	}
}


/*
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
*/
