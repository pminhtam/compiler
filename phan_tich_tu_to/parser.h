#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include "scanning.h"
#include <stdlib.h>

using namespace std;

int tokens_size = 0;
int i_token = 0;		// token thu i
int i_number = 0,i_ident = 0;
TokenType Token;


TokenType get_token_parse(int show);
void error(string s);
void factor();
void term();
void expression();
void condition();
void statement();
void block();
void program();



TokenType get_token_parse(int show = 1){
	if (i_token<tokens_size){
		if(tokens[i_token] == NUMBER){
			cout<<" "<<numbers[i_number]<<" ";
			i_number++;
		}
		else if( tokens[i_token] == IDENT){
			cout<<" "<<idents[i_ident]<<" ";
			i_ident++;
		}
		else{
			if(show!=0 || tokens[i_token] != SEMICOLON){
				cout<<" "<<type_words[(int) tokens[i_token]]<<" ";
				if(tokens[i_token] == SEMICOLON){
					cout<<endl;
				}
			}
			
		}
		return tokens[i_token++];
	}
	exit(EXIT_SUCCESS);
}



void error(string s){
	cout<<"\n Loi:  "<<s<<"  dong:  "<<lines[i_token>1 ? i_token -2: 0 ]<<"  cot:  "<<columns[i_token>0 ? i_token-2: 0 ]<<endl;
//	cout<<"\n Loi:  "<<s<<"  dong:  "<<lines[i_token -1]<<"  cot:  "<<columns[i_token -1]<<endl;

	exit(EXIT_SUCCESS);
}

void factor(){
	if(Token == IDENT){
		Token = get_token_parse();
		if(Token == LBRACK){
			Token = get_token_parse();
			expression();
			if(Token == RBRACK){
				Token = get_token_parse();
			}else{
				error("factor: Thieu dau ] ");
			}
		}
	}
	else if(Token == NUMBER){
		Token = get_token_parse();
	}
	else if(Token == LPARENT){
		Token = get_token_parse();
		expression();
		if(Token == RPARENT){
			Token = get_token_parse();
		}
		else{
			error("factor: Thieu dau ) ");
		}
	}
}
void term(){
	factor();
	while(Token == TIMES || Token == SLASH)
	{
		Token = get_token_parse();
		factor();
	}

}

void expression(){
	if(Token== PLUS || Token == MINUS)
		Token = get_token_parse();
	term();
	while(Token == PLUS || Token == MINUS){
		Token = get_token_parse();
		term();
	}

}

void condition(){
	if(Token == ODD){
		Token = get_token_parse();
		expression();
	}
	else{
		expression();
		if(Token == EQU || Token == NEQ || Token == LSS ||   
	       Token == LEQ || Token == GRT || Token == GEQ){
			Token = get_token_parse();
			expression();
		}else	{
			error("condition: Thieu dau trong bieu thuc ");
		}
	}
}

void statement(){
	
//	cout<<"Token statement nay  "<<type_words[(int) Token]<<endl;
	
	
	if(Token==IDENT){	//variable := <expression>
		Token =get_token_parse();	//array variable ?
		if(Token==ASSIGN){
			Token = get_token_parse();
			expression();
		}else error("statement: IDENT:  Thieu toan tu gan");
	} 
	else if (Token == CALL){
		Token= get_token_parse();
		if(Token == IDENT){
			Token = get_token_parse();
			if(Token == LPARENT){
				Token = get_token_parse();
				expression();
				while(Token==COMMA){
					Token = get_token_parse();
					expression();
				}
				if(Token = RPARENT) Token = get_token_parse();
				else error("statement:  CALL: Thieu dau )");
			}
		}
		else error("statement : CALL:  Thieu ten thu tuc ham");
	}
	else if(Token == BEGIN){
		Token = get_token_parse();
		statement();
		while (Token == SEMICOLON) {
			Token = get_token_parse();
			statement();
		}
		if(Token==END) Token = get_token_parse();
		else error("statement :  BEGIN:  Thieu tu khoa END hoac cau lenh thieu ;");
	}
	else if(Token == IF){
		Token = get_token_parse();
		condition();
		if(Token == THEN){
			Token = get_token_parse();
			statement();
			if(Token == ELSE){
				Token = get_token_parse();
				statement();
			}
		}
		else{
			error("statement : IF: thieu THEN");
		}
	}
	else if(Token == WHILE){
		Token = get_token_parse();
		condition();
		if(Token == DO){
			Token = get_token_parse();
			statement();
		}
		else{
			error("statement : WHILE: thieu DO");
		}
	}
	else if(Token == FOR){
		Token = get_token_parse();
		if(Token == IDENT){
			Token = get_token_parse();
			if(Token == ASSIGN){
				Token = get_token_parse();
				expression();
				if(Token == TO){
					Token = get_token_parse();
					expression();
					if(Token == DO){
						Token = get_token_parse();
						statement();
					}
					else{
						error("statement : FOR thieu DO");
					}
				}
				else{
					error("statement : FOR thieu TO");
				}
			}
			else{
				error("statement : FOR thieu ASSIGN");
			}
		}
		else{
			error("statement : FOR thieu IDENT");
		}
	}
	
//		cout<<"Token cuoi statement nay  "<<type_words[(int) Token]<<endl;


} 

void block(){
	
//	cout<<"Token block nay  "<<type_words[(int) Token]<<endl;
	
	if(Token == CONST){
		do{
			Token = get_token_parse();
			if (Token==IDENT){
				Token = get_token_parse();
				if(Token == EQU){
					Token = get_token_parse();
					if(Token == NUMBER){
						Token = get_token_parse();	
						if(Token == SEMICOLON){
							Token = get_token_parse();	
							break;	
						}
					}
					else{
						error("block: CONST: Thieu gia tri");
					}
				}
				else{
					error("block: CONST: Thieu toan tu EQU");
				}
			}
			else{
				error("block: CONST: thieu dinh danh");
			}
		}while(Token==COMMA);
	}
	
	if(Token == VAR){
		do{
			Token = get_token_parse();
			if (Token==IDENT){
				Token = get_token_parse();
				if(Token == LBRACK){
					Token = get_token_parse();
					if(Token == NUMBER){
						Token = get_token_parse();	
						if(Token == RBRACK){
							Token = get_token_parse();	
						}
						else{
							error("block: VAR: Thieu dau ]");
						}
					}
					else{
						error("block: VAR: Thieu gia tri");
					}
				}
			}
			else{
				error("block: VAR: thieu dinh danh");
			}
		}while(Token==COMMA);
		if(Token == SEMICOLON){
//			Token = get_token_parse();
			// nhieu dau ; long nhau
			do{
				Token = get_token_parse(0);
			} while(Token == SEMICOLON);
		}
		else{
			error("block: VAR: thieu ;");
		}
	}
	
	while(Token == PROCEDURE){		
		Token = get_token_parse();
		if (Token==IDENT){
			Token = get_token_parse();
			
			if(Token == LPARENT){		//  (
				do{
					Token = get_token_parse(0);
					if(Token == VAR){
						Token = get_token_parse();
					}
					if(Token == IDENT){
						Token = get_token_parse();
					}
					else{
						error("block: PROCEDURE: thieu IDENT");
					}
				}while(Token == SEMICOLON);
				if(Token == RPARENT){
					Token = get_token_parse();
				}
				else{
					error("block: PROCEDURE: thieu dau )");
				}
				
			}
			if(Token == SEMICOLON){
				// nhieu dau ; long nhau
				do{
					Token = get_token_parse(0);
				} while(Token == SEMICOLON);
				
				
				block();
				if(Token == SEMICOLON){					
					do{
						Token = get_token_parse(0);
					} while(Token == SEMICOLON);
				
				
				}
				else{
					error("block: PROCEDURE: thieu dau ; sau block BEGIN -- END");
				}
			}
			else{
				error("block: PROCEDURE: thieu dau ; sau PROCEDURE");
			}
		}
		else{
			error("block: PROCEDURE: thieu ten (ident) PROCEDURE");
		}
	}
	
	
	if(Token == BEGIN){
		Token = get_token_parse();
		statement();
		while(Token == SEMICOLON){			
			do{
					Token = get_token_parse(0);
			} while(Token == SEMICOLON);
					
			statement();
		}
		if(Token == END){
			Token = get_token_parse();			
		}else{
			error("block: BEGIN: thieu END  hoac cau lenh thieu ;");
		}
	}
	
}

void program(){
	if(Token==PROGRAM){
		Token = get_token_parse();
		if (Token==IDENT){
			Token = get_token_parse();
			if(Token==SEMICOLON){
				Token = get_token_parse();
				block();

				while(Token == SEMICOLON){
					Token = get_token_parse(0);
				} 
				
				if(Token==PERIOD)
					cout<<" \n Thanh cong "<<endl;
				else error(" program  : Thieu dau cham");
			}else error("program  : Thieu dau cham phay");
		}else error(" program  :Thieu ten chuong trinh");
	}else error(" program:  Thieu tu khoa Program");

}

/*
int main(int argc, char **args){
	if (argc <= 1) 
		return -1;	
	
	get_tokens(args[1]);
	tokens_size = tokens.size();

	Token = get_token_parse();

	program();
	

}
*/
