#include "scanning.h"





char getCh(ifstream& sfile){
	char c;
	if(!sfile.eof()){
		sfile>>c;
		column++;
		
//		if(c == '\n'){
//			cout<<"   "<<endl;
//		}
		/*
		while(c == '\n' && !sfile.eof()){
			line++;
			column=0;
			sfile>>c;
		}
		*/
	}
	else{
		return 0;
	}
	char upper_c =toupper(c);
//	cout<<upper_c<<"   ";
	return upper_c;
}


int check_letter(char ch){
//	cout<<"letter:  "<<ch<<endl;
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
	
	// kiem tra do dai xau
	string s_cut;
	if(s.length() > max_ident){
		for(i=0;i<max_ident;i++){		// cat lay 9 ky tu
			s_cut+=s[i];
		}
		idents.push_back(s_cut);
	}
	else{
		idents.push_back(s);
	}
	return IDENT;
}

TokenType test_number(string s){
	int value = 0;
	int i= 0; 
	int L = s.length();
	
	if(L>max_num){
		cout<<"\n Loi:  So qua lon"<<"  dong:  "<<line<<"  cot:  "<<column<<endl;
		exit(EXIT_SUCCESS);
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
			numbers.push_back(value);
			return NUMBER; 
		}
	}
	return NONE;
	
}

TokenType getToken(ifstream& sfile){
	get_token:
	
	while(ch == ' ' || ch==9 || ch =='\n'){
		if(ch=='\n'){
			line++;
			column=0;
		}
		ch = getCh(sfile);
	}
	
	string s;
	if(check_letter(ch)){
		s+=ch;
		ch = getCh(sfile);
//		cout<<"ident"<<ch<<endl;
		while(check_letter(ch) || check_digit(ch)){
			s+=ch;
			ch = getCh(sfile);
		};
		return test_ident(s);
	}
	
	else if(check_digit(ch)){
		s+=ch;
		ch = getCh(sfile);
		while(check_digit(ch)){
			s+=ch;
			ch = getCh(sfile);
		};
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
		
		if(ch == '*'){
			comment:
			ch = getCh(sfile);
			while(ch != '*'){
				
				ch = getCh(sfile);
			}
			ch = getCh(sfile);
			if(ch == ')'){
				ch = getCh(sfile);
//				cout<<"Het comment"<<endl;
				goto get_token;
			}
			else{
				goto comment;
			}
		}
		
		
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
//		cout<<"SEMICOLON"<<endl;
		return SEMICOLON;
	}
	
	else{
//		cout<<"het  "<<(int)ch<<endl;
		if(!sfile.eof()){
			cout<<"Xuat hien ky tu la hoac file bi loi"<<"  dong:  "<<line<<"  cot:  "<<column<<endl;
			exit(EXIT_SUCCESS);
		}
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


void get_tokens(char* file_name){
	ifstream sfile;

	sfile.open(file_name,ios_base::in);
	sfile.unsetf(ios_base::skipws);	
	TokenType token;

	do{
		token = getToken(sfile);
		tokens.push_back(token);
		lines.push_back(line);
		columns.push_back(column);
		
	}while(token!=NONE);
}

