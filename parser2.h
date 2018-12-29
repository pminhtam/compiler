#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include "scanning.h"
#include <stdlib.h>
#include "asm.h"
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
		FindResult result_entry = find_entry(get_ident());
		if(result_entry.entry==NULL){
				error("factor:  scope:  Bien "+get_ident() + "  chua duoc khai bao" );
		}
		if(result_entry.entry->type == TYPE_PROC){
			error("factor:  scope:   Bien "+get_ident() + "  là procedure, khong the dung trong factor " );
		}
		
		
		Token = get_token_parse();
		
		if(Token == LBRACK){
			if(result_entry.entry->is_array == false){
				error("factor:  scope:   Bien "+get_ident() + "  khong phai la bien kieu array" );
			}
			Token = get_token_parse();
			expression();
			if(Token == RBRACK){
				Token = get_token_parse();
			}else{
				error("factor: Thieu dau ] ");
			}
			
			asm_LA(result_entry.depth,result_entry.entry->offset);
			asm_ADD();
			asm_LI();

		}
		else{
			if(result_entry.entry->is_array == true){
				error("factor:  scope:   Bien "+get_ident() + "  la mot array, khong the dung " );
			}
			
			if(result_entry.entry->type == TYPE_INT){
				asm_LV(result_entry.depth,result_entry.entry->offset);
			}
			else if(result_entry.entry->type == TYPE_CONST){
				asm_LC(result_entry.entry->const_val);
			}
		}
	}
	else if(Token == NUMBER){
		
		asm_LC(get_number());
		
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
	TokenType op;
	factor();
	while(Token == TIMES || Token == SLASH)
	{
		op = Token;
		Token = get_token_parse();
		factor();
		if(op == TIMES){
			asm_MUL();
		}
		else{
			asm_DIV();
		}
	}

}

void expression(){
	TokenType op;
	if(Token== PLUS || Token == MINUS){
		op = Token;
		Token = get_token_parse();
	}
	term();
	if(op == PLUS){
		asm_ADD();
	}
	else if(op == MINUS){
		asm_NEG();
	}
	while(Token == PLUS || Token == MINUS){
		op = Token;
		Token = get_token_parse();
		term();
		if(op == PLUS){
			asm_ADD();
		}
		else if(op == MINUS){
			asm_SUB();
		}
	}

}

void condition(){
	TokenType op;
	if(Token == ODD){
		Token = get_token_parse();
		expression();
		asm_ODD();
	}
	else{
		expression();
		if(Token == EQU || Token == NEQ || Token == LSS ||   
	       Token == LEQ || Token == GRT || Token == GEQ){
	       	op = Token;
	       	
			Token = get_token_parse();
			expression();
			
			switch(op){
				case EQU:
					asm_EQ();
					break;
				case NEQ:
					asm_NE();
					break;
				case LSS:
					asm_LT();
					break;
				case LEQ:
					asm_LE();
					break;
				case GRT:
					asm_GT();
					break;
				case GEQ:
					asm_GE();
					break;
			}
		}else	{
			error("condition: Thieu toan tu so sanh ");
		}
	}
}

void statement(){	
	
	if(Token==IDENT){	//variable := <expression>
		Token =get_token_parse();	//array variable ?
		//scope
		FindResult result_entry = find_entry(get_ident());
		if(result_entry.entry==NULL){
			error("statement: IDENT:  scope: Bien "+get_ident() + "  chua duoc khai bao" );
		}
		else if(result_entry.entry->type != TYPE_INT){
			error("statement:  IDENT:   Ten "+get_ident() + "  khong phai la bien var " );
		}
//		else{
		
		if(Token == LBRACK){
			
			if(result_entry.entry->is_array == false){
				error("statement: IDENT:  scope:  Bien "+get_ident() + "  khong phai la bien kieu array" );
			}
			Token = get_token_parse();
			expression();
						
			if(Token==RBRACK){
				Token = get_token_parse();
			}else{
				error("statement: IDENT:  dong ngoac ]");
			}	
			asm_LA(result_entry.depth,result_entry.entry->offset);
			asm_ADD();
		}
		else{
			if(result_entry.entry->is_array == true){
				error("statement: IDENT:  scope:  Bien "+get_ident() + " la bien kieu array.  Khong the gan truc tiep" );
			}
			// gan gia tri cho 1 toan hang toan hang 
			asm_LA(result_entry.depth,result_entry.entry->offset);
		}
//		}
		
		if(Token==ASSIGN){
			Token = get_token_parse();
			expression();
			asm_ST();
		}
		else{
			error("statement: IDENT:  Thieu toan tu gan");
		}
	} 
	else if (Token == CALL){
		Token= get_token_parse();
		if(Token == IDENT){
			//scope
			string procedure_name = get_ident();
			if(procedure_name=="WRITEI"){
				Token = get_token_parse();
				if(Token == LPARENT){
					Token = get_token_parse();
					expression();
					
					asm_WRI();
					if(Token == RPARENT){
						Token = get_token_parse();
					}
					else error("statement:  CALL: Thieu dau )");
				}
			}
			else if(procedure_name=="WRITELN"){
				Token = get_token_parse();
				asm_WLN();
			}
			else if(procedure_name == "READI"){
				Token = get_token_parse();
				if(Token == LPARENT){
					Token = get_token_parse();
					if(Token = IDENT){
						Token = get_token_parse();
						FindResult result_entry = find_entry(get_ident());
						if(result_entry.entry==NULL){
							error("statement: IDENT:  scope: Bien "+get_ident() + "  chua duoc khai bao" );
						}
						else if(result_entry.entry->type != TYPE_INT){
							error("statement:  IDENT:   Ten "+get_ident() + "  khong phai la bien var " );
						}
						if(Token == LBRACK){
							if(result_entry.entry->is_array == false){
								error("statement: IDENT:  scope:  Bien "+get_ident() + "  khong phai la bien kieu array" );
							}
							Token = get_token_parse();
							expression();
										
							if(Token==RBRACK){
								Token = get_token_parse();
							}else{
								error("statement: IDENT:  dong ngoac ]");
							}	
							asm_LA(result_entry.depth,result_entry.entry->offset);
							asm_ADD();
						}
						else{
							if(result_entry.entry->is_array == true){
								error("statement: IDENT:  scope:  Bien "+get_ident() + " la bien kieu array.  Khong the gan truc tiep" );
							}
							// gan gia tri cho 1 toan hang toan hang 
							asm_LA(result_entry.depth,result_entry.entry->offset);
						}
						asm_RI();
					}
					else{
						error("statement:  CALL: readi sai tham so truyen vao)");
					}
					if(Token == RPARENT){
						Token = get_token_parse();
					}
					else error("statement:  CALL: Thieu dau )");
				}
			}
			else{
				asm_INT(4);
				
				FindResult result_entry_procedure = find_entry(get_ident());
				
				
				int num_var_call = 0;
				if(result_entry_procedure.entry==NULL){
					error("statement: CALL: scope:   Ten "+get_ident() + "  chua duoc khai bao" );
				}
				else if(result_entry_procedure.entry->type != TYPE_PROC){
					error("statement: CALL: scope:   Ten "+get_ident() + "  khong phai la procedure" );
				}
				Token = get_token_parse();
				if(Token == LPARENT){
					int i0 = i_ident;		// xem expression co goi ident nao khong
					Token = get_token_parse();
					expression();
					int i1 = i_ident;
					if(result_entry_procedure.entry->procedure_table->entries.size()>num_var_call+1){
						if(result_entry_procedure.entry->procedure_table->entries[num_var_call].is_reference){
							if(i0==i1){
								error("statement:  CALL:  scope:   Tham so truyen vao khong phai la bien tham chieu");
							}
							else{
								FindResult result_entry = find_entry(get_ident());
								if(result_entry.entry->type != TYPE_INT){
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
						
						if(result_entry_procedure.entry->procedure_table->entries.size()>num_var_call+1){
							if(result_entry_procedure.entry->procedure_table->entries[num_var_call].is_reference){
								if(i0==i1){
									error("statement:  CALL:  scope:   Tham so truyen vao khong phai la bien tham chieu");
								}
								else{
									if(i1-i0==1){
										FindResult result_entry = find_entry(get_ident());
										if(result_entry.entry->type != TYPE_INT){
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
				if(num_var_call != result_entry_procedure.entry->procedure_table->num_var){
					cout<<"\nProcedure "<<result_entry_procedure.entry->name<<"  can  "<<result_entry_procedure.entry->procedure_table->num_var<<" tham so. Truyen vao "<<num_var_call<<" tham so"<<endl;
					error("statement:  CALL:  scope:   Sai so luong tham so truyen vao procedure");
				}
				
				asm_DCT(4+num_var_call);
				
				asm_CALL(result_entry_procedure.depth,result_entry_procedure.entry->procedure_table->procedure_addr);
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
		 
		int false_jump = Code.size();
		asm_FJ(0);
		
		if(Token == THEN){
			Token = get_token_parse();
			statement();
			
			int L1 = Code.size();
			
			if(Token == ELSE){
				int jump = Code.size();
				asm_J(0);
				L1 = Code.size();
				Token = get_token_parse();
				statement();
				int L2 = Code.size();
				Code[jump].q = L2;
			}
			
			Code[false_jump].q = L1;
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
		
		int L1 = Code.size();
		condition();
		
		int false_jump = Code.size();
		asm_FJ(0);
		
		if(Token == DO){
			Token = get_token_parse();
			statement();
			asm_J(L1);
			int L2 = Code.size();
			
			Code[false_jump].q = L2;	
		}
		else{
			error("statement : WHILE: thieu DO");
		}
	}
	else if(Token == FOR){
		int L1,L2;
		int false_jump_L2;
		Token = get_token_parse();
		if(Token == IDENT){
			// scope
			FindResult result_entry = find_entry(get_ident());
			if(result_entry.entry==NULL){
				error("statement: FOR:  scope:  Ten "+get_ident() + "  chua duoc khai bao" );
			}
			else if(result_entry.entry->type != TYPE_INT){
				error("statement: FOR:  scope:  Ten "+get_ident() + "  khong phai la bien nen khong the gan gia tri" );
			}
			else if(result_entry.entry->is_array){
				error("statement: FOR:  scope:  Ten "+get_ident() + "  la mot mang khong dung duoc trong vong for " );
			}
			asm_LA(result_entry.depth,result_entry.entry->offset);
			asm_CV();
			
			Token = get_token_parse();
			if(Token == ASSIGN){
				Token = get_token_parse();
				if(Token == TO){
					error("statement : FOR thieu expression");
				}
				expression();
				
				asm_ST();
				L1 = Code.size();

				
				if(Token == TO){
					asm_CV();
					asm_LI();
					Token = get_token_parse();
					if(Token == DO){
						error("statement : FOR thieu expression");
					}
					expression();
					asm_LE();
					false_jump_L2 = Code.size();
					asm_FJ(0);
					if(Token == DO){
						Token = get_token_parse();
						statement();
						asm_CV();
						asm_CV();
						asm_LI();
						asm_LC(1);
						asm_ADD();
						asm_ST();
						asm_J(L1);
						
						L2 = Code.size();
						Code[false_jump_L2].q = L2;
						asm_DCT(1);
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
						if(find_entry(get_ident()).entry!=NULL){
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
			if(find_entry(get_ident()).entry!=NULL){
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
				
				asm_EP();
				
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
	
	current_table->procedure_addr = Code.size();
	asm_INT(current_table->mem_size);	
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
		init_asm();
		Token = get_token_parse();
		if (Token==IDENT){
			//scope
			init_table(get_ident());
			
			
			Token = get_token_parse();
			if(Token==SEMICOLON){
				Token = get_token_parse();
				block();
				
								
				if(Token==PERIOD){
					cout<<" \n Thanh cong "<<endl;
					Code[0].p = Code[0].q = current_table->procedure_addr;
					asm_BP();
				}
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
