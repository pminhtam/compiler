#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <cstdlib>

using namespace std;



enum Type{
	TYPE_INT,
	TYPE_PROC,
	TYPE_CONST,
};

struct Entry{
	string name;
	Type type;
	int offset;
	
	int var_val;
	int is_reference;
	int is_array;
	int arr_size;
	
	int const_val;
	
	struct Table* procedure_table;
	
};


struct Table{
	string name;
	Table *parent;
	int procedure_addr;
	int mem_size;
	vector<Entry> entries;
	int num_var;		// so luong parameter trong mot procedure
};

struct FindResult{
	Entry *entry;
	int depth;	
};


void init_table(string name);
void make_table(string name);
void table_add_var(string name, bool is_reference,bool is_array,int arr_size);
void table_add_const(string name,int const_val);
void table_add_proc(string name,Table* table);
FindResult find_entry(string name);
Entry *find_entry_procedure(string name,Table *table);


static Table* root_table;
static Table* current_table;

void init_table(string name){
	root_table = new Table();
	root_table->name = name;
	root_table->parent == NULL;
	root_table->mem_size = 4;
	current_table = root_table;
}

void make_table(string name){
	Table* table = new Table();
	table->parent = current_table;
	table->name = name;
	table->mem_size = 4;			// 4 thanh ghi dac biet
	table_add_proc(name,table);
//	cout<<"cha la  "<<table->parent->name<<endl;
	current_table = table;
}

void table_add_var(string name, bool is_reference,bool is_array,int arr_size){
	Entry e;
	e.name = name;
	e.type = TYPE_INT;
	e.is_reference = is_reference;
	e.is_array = is_array;
	e.arr_size = arr_size;
	e.offset = current_table->mem_size;
	if (is_array) 
        current_table->mem_size += arr_size * 1;
    else
        current_table->mem_size += 1;
    current_table->entries.push_back(e);
}

void table_add_const(string name,int const_val){
	Entry e;
	e.name = name;
	e.type = TYPE_CONST;
	e.const_val = const_val;
    e.is_array = false;
    e.is_reference = false;
    current_table->entries.push_back(e);
}

void table_add_proc(string name,Table* table){
	Entry e;
	e.name = name;
	e.type = TYPE_PROC;
	e.procedure_table = table;
	e.is_array = false;
	e.is_reference = false;
//	cout<<"table_add_proc  "<<name<<endl;    
//	cout<<"current table  "<<current_table->name<<endl;
    current_table->entries.push_back(e);
}


FindResult find_entry(string name){
	Table *table = current_table;
	int depth = 0;
	FindResult result;
	while(table != NULL){
		for(int i = 0;i<table->entries.size();i++){
//			cout<<table->entries[i].name<<endl;
			if(name == table->entries[i].name){
				result.entry = &table->entries[i];
				result.depth = depth;
				return result;
			}
		}
		table = table->parent;
		depth +=1;
	}
	result.entry = NULL;
	return result;
}

Entry *find_entry_procedure(string name,Table *table ){
	for(int i = 0;i<table->entries.size();i++){
		if(name == table->entries[i].name){
//			cout<<"Dang tim bang"<<table->name<<endl;
			
			return &table->entries[i];
		}
	}
	return NULL;
}

/*
int main(){
	init_table();
	cout<<current_table->name;
	table_add_var("a",false,false,0);
	table_add_var("b",false,false,0);
	table_add_var("c",false,false,0);
	table_add_var("d",false,false,0);
	Entry *e = find("b");
	cout<<e->name;
	
}
*/
