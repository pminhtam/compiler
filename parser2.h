#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include "scanning.h"
#include <stdlib.h>

//xác dinh so luong tham so cua thu tuc
// tham tri va tham bien

using namespace std;

int tokens_size = 0;
int i_token = 0;		// token thu i
int i_number = 0,i_ident = 0;
TokenType Token;


TokenType get_token_parse();
void error(string s);
void factor();
void term();
void expression();
void condition();
void statement();
void block();
void program();



TokenType get_token_parse(){
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
			cout<<" "<<type_words[(int) tokens[i_token]]<<" ";
			if(tokens[i_token] == SEMICOLON){
				cout<<endl;
			}
		}	
		return tokens[i_token++];
	}
	exit(EXIT_SUCCESS);
}

string get_ident(){
	return idents[i_ident-1];
}

int get_number(){
	return numbers[i_number-1];
}



void error(string s){
	cout<<"\n Loi:  "<<s<<"  dong:  "<<lines[i_token>1 ? i_token -2: 0 ]<<"  cot:  "<<columns[i_token>0 ? i_token-2: 0 ]<<endl;
//	cout<<"\n Loi:  "<<s<<"  dong:  "<<lines[i_token -1]<<"  cot:  "<<columns[i_token -1]<<endl;

	exit(EXIT_SUCCESS);
}

void factor(){
	if(Token == IDENT){
		Entry *entry = find_entry(get_ident());
		if(entry==NULL){
				error("factor:  scope:  Bien "+get_ident() + "  chua duoc khai bao" );
		}
		if(entry->type == TYPE_PROC){
			error("factor:  scope:   Bien "+get_ident() + "  là procedure, khong the dung trong factor " );
		}
		Token = get_token_parse();
		if(Token == LBRACK){
			if(entry->is_array == false){
				error("factor:  scope:   Bien "+get_ident() + "  khong phai la bien kieu array" );
			}
			Token = get_token_parse();
			expression();
			if(Token == RBRACK){
				Token = get_token_parse();
			}else{
				error("factor: Thieu dau ] ");
			}
		}
		else{
			if(entry->is_array == true){
				error("factor:  scope:   Bien "+get_ident() + "  la mot array, khong the dung " );
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
	else{
		error("factor: Khong phai factor ");
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
			error("condition: Thieu toan tu so sanh ");
		}
	}
}

void statement(){	
	
	if(Token==IDENT){	//variable := <expression>
		Token =get_token_parse();	//array variable ?
		//scope
		Entry *entry = find_entry(get_ident());
		if(entry==NULL){
			error("statement: IDENT:  scope: Bien "+get_ident() + "  chua duoc khai bao" );
		}
		else if(entry->type != TYPE_INT){
			error("statement:  IDENT:   Ten "+get_ident() + "  khong phai la bien var " );
		}
//		else{

		if(Token == LBRACK){
			if(entry->is_array == false){
				error("statement: IDENT:  scope:  Bien "+get_ident() + "  khong phai la bien kieu array" );
			}
			Token = get_token_parse();
			expression();
			if(Token==RBRACK){
				Token = get_token_parse();
			}else{
				error("statement: IDENT:  dong ngoac ]");
			}	
		}
		else{
			if(entry->is_array == true){
				error("statement: IDENT:  scope:  Bien "+get_ident() + " la bien kieu array.  Khong the gan truc tiep" );
			}
		}
//		}
		
		if(Token==ASSIGN){
			Token = get_token_parse();
			expression();
		}
		else{
			error("statement: IDENT:  Thieu toan tu gan");
		}
	} 
	else if (Token == CALL){
		Token= get_token_parse();
		if(Token == IDENT){
			//scope
			Entry *entry_procedure = find_entry(get_ident());
			int num_var_call = 0;
			if(entry_procedure==NULL){
				error("statement: CALL: scope:   Ten "+get_ident() + "  chua duoc khai bao" );
			}
			else if(entry_procedure->type != TYPE_PROC){
				error("statement: CALL: scope:   Ten "+get_ident() + "  khong phai la procedure" );
			}
			Token = get_token_parse();
			if(Token == LPARENT){
				int i0 = i_ident;		// xem expression co goi ident nao khong
				Token = get_token_parse();
				expression();
				int i1 = i_ident;
				if(entry_procedure->procedure_table->entries.size()>num_var_call+1){
					if(entry_procedure->procedure_table->entries[num_var_call].is_reference){
						if(i0==i1){
							error("statement:  CALL:  scope:   Tham so truyen vao khong phai la bien tham chieu");
						}
						else{
							Entry *entry = find_entry(get_ident());
							if(entry->type != TYPE_INT){
								error("statement:  CALL:  scope:   Tham so truyen vao khong phai la bien tham chieu 2");
							}
						}
					}
				}
				num_var_call +=1;
				while(Token==COMMA){
					i0 = i_ident;
					Token = get_token_parse();
					expression();
					i1 = i_ident;
					
					if(entry_procedure->procedure_table->entries.size()>num_var_call+1){
						if(entry_procedure->procedure_table->entries[num_var_call].is_reference){
							if(i0==i1){
								error("statement:  CALL:  scope:   Tham so truyen vao khong phai la bien tham chieu");
							}
							else{
								if(i1-i0==1){
									Entry *entry = find_entry(get_ident());
									if(entry->type != TYPE_INT){
										error("statement:  CALL:  scope:   Tham so truyen vao khong phai la bien tham chieu 2");
									}
								}
								else{
									error("statement:  CALL:  scope:   Tham so truyen vao khong phai la bien tham chieu 3");
								}
							}
						}
					}
					num_var_call +=1;
				}
				if(Token == RPARENT){
					Token = get_token_parse();
				}
				else error("statement:  CALL: Thieu dau )");
			}
			if(num_var_call != entry_procedure->procedure_table->num_var){
				cout<<"\nProcedure "<<entry_procedure->name<<"  can  "<<entry_procedure->procedure_table->num_var<<" tham so. Truyen vao "<<num_var_call<<" tham so"<<endl;
				error("statement:  CALL:  scope:   Sai so luong tham so truyen vao procedure");
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
		if(Token == THEN){
			error("statement : IF: thieu condition");
		}
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
		if(Token == DO){
			error("statement : WHILE: thieu condition");
		}
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
			// scope
			Entry *entry = find_entry(get_ident());
			if(entry==NULL){
				error("statement: FOR:  scope:  Ten "+get_ident() + "  chua duoc khai bao" );
			}
			else if(entry->type != TYPE_INT){
				error("statement: FOR:  scope:  Ten "+get_ident() + "  khong phai la bien nen khong the gan gia tri" );
			}
			else if(entry->is_array){
				error("statement: FOR:  scope:  Ten "+get_ident() + "  la mot mang khong dung duoc trong vong for " );
			}
			Token = get_token_parse();
			if(Token == ASSIGN){
				Token = get_token_parse();
				if(Token == TO){
					error("statement : FOR thieu expression");
				}
				expression();
				if(Token == TO){
					Token = get_token_parse();
					if(Token == DO){
						error("statement : FOR thieu expression");
					}
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

} 

void block(){
	
	if(Token == CONST){
		do{
			Token = get_token_parse();
			if (Token==IDENT){
				Token = get_token_parse();
				if(Token == EQU){
					Token = get_token_parse();
					if(Token == NUMBER){
						//scope
						if(find_entry(get_ident())!=NULL){
							error("block: CONST:  scope:  Ten "+get_ident() + "  da duoc khai bao" );
						}
						table_add_const(get_ident(),get_number());
						
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
							//scope
							if(find_entry_procedure(get_ident(),current_table)!=NULL){
								error("block: VAR:  scope:  Ten "+get_ident() + "  da duoc khai bao" );
							}
							table_add_var(get_ident(),false,true,get_number());
							Token = get_token_parse();	
						}
						else{
							error("block: VAR: Thieu dau ]");
						}
					}
					else{
						error("block: VAR: Thieu gia tri NUMBER");
					}
				}
				else{
					//scope
					if(find_entry_procedure(get_ident(),current_table)!=NULL){
						error("block: VAR:  scope:  Ten "+get_ident() + "  da duoc khai bao 2" );
					}
					table_add_var(get_ident(),false,false,0);
				}
			}
			else{
				error("block: VAR: thieu dinh danh IDENT");
			}
		}while(Token==COMMA);
		if(Token == SEMICOLON){
			Token = get_token_parse();
		}
		else{
			error("block: VAR: thieu ;");
		}
	}
	
	while(Token == PROCEDURE){		
		Token = get_token_parse();
		//scope
		string procedure_name;
		int num_var = 0;
		if (Token==IDENT){
			//scope
			if(find_entry(get_ident())!=NULL){
				error("block: PROCEDURE:  scope:  Ten "+get_ident() + "  da duoc khai bao" );
			}
			string procedure_name = get_ident();
			make_table(procedure_name);		// tao bang moi cho procedure
			
			
			Token = get_token_parse();
			
			//scope
			bool is_reference;
			
			if(Token == LPARENT){		//  (
				do{
					Token = get_token_parse();
					if(Token == VAR){
						Token = get_token_parse();
						is_reference = true;
					}
					if(Token == IDENT){
						Token = get_token_parse();
						//scope
						num_var +=1;
						if(find_entry_procedure(get_ident(),current_table)!=NULL){
							error("block: PROCEDURE:  scope:  Ten "+get_ident() + "  da duoc khai bao 2" );
						}
						table_add_var(get_ident(),is_reference,false,0);
						
					}
					else{
						error("block: PROCEDURE: thieu IDENT");
					}
					is_reference = false;
				}while(Token == SEMICOLON);
				if(Token == RPARENT){
					Token = get_token_parse();
				}
				else{
					error("block: PROCEDURE: thieu dau )");
				}
				
			}
			if(Token == SEMICOLON){
				Token = get_token_parse();				
				
				block();
				if(Token == SEMICOLON){				
					Token = get_token_parse();
				
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
		current_table->num_var = num_var;
		current_table = current_table->parent;
	}
	
	
	if(Token == BEGIN){
		Token = get_token_parse();			
		statement();
		while(Token == SEMICOLON){			

			Token = get_token_parse();		
			statement();
		}
		if(Token == END){
			Token = get_token_parse();			
		}else{
			int i0 = i_token;
			statement();
			int i1 = i_token;
			if(i0 == i1){
				error("block: BEGIN: thieu END ");
			}
			else{
				i_token = i0;
				error("block: BEGIN: cau lenh thieu ; ");
			}
//			error("block: BEGIN: thieu END  hoac cau lenh thieu ;");
		}
	}
	else{
		error("block: BEGIN: thieu BEGIN");	
	}
	
}

void program(){
	if(Token==PROGRAM){
		Token = get_token_parse();
		if (Token==IDENT){
			//scope
			init_table(get_ident());
			
			
			Token = get_token_parse();
			if(Token==SEMICOLON){
				Token = get_token_parse();
				block();
				
								
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
