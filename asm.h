
#define MAX_CODE 1000
#define MAX_DATA 1000

typedef enum {
  OP_LA,   // Load Address:		0
  OP_LV,   // Load Value:		1
  OP_LC,   // load Constant 	2
  OP_LI,   // Load Indirect 	3
  OP_INT,  // Increment t 		4
  OP_DCT,  // Decrement t		5
  OP_J,    // Jump 				6
  OP_FJ,   // False Jump 		7
  OP_HLT,  // Halt 				8
  OP_ST,   // Store 			9
  OP_CALL, // Call				10
  OP_EP,   // Exit Procedure 	11
  OP_EF,   // Exit Function 	12
  OP_RC,   // Read Char 		13
  OP_RI,   // Read Integer		14
  OP_WRC,  // Write Char    	15
  OP_WRI,  // Write Int			16
  OP_WLN,  // WriteLN 			17
  OP_ADD,  // Add 				18
  OP_SUB,  // Substract			19
  OP_MUL,  // Multiple			20
  OP_DIV,  // Divide			21
  OP_NEG,  // Negative			22
  OP_CV,   // Copy Top			23
  OP_EQ,   // Equal				24
  OP_NE,   // Not Equal			25
  OP_GT,   // Greater			26
  OP_LT,   // Less				27
  OP_GE,   // Greater or Equal	28
  OP_LE,   // Less or Equal		29
  OP_BP,   // Break point.		30
  OP_ODD
} OpCode ;
typedef struct{
  	OpCode Op;
  	int p;
  	int q;
} Instruction;

vector<Instruction> Code;
int Stack[MAX_DATA];
int max_code= 0;

void show_instruction(int max){
	cout<<"\n============================================="<<endl;
  int Exit = 0;
  int pc = 0;
  	do {
  		cout<< pc <<"    " ;
     	switch (Code[pc].Op) {
	    	case OP_LA:   
	        	cout<<"LA"<<"   "<<Code[pc].p<<",   "<<Code[pc].q<<endl;
	        	break;
	     	case OP_LV:   
	         	cout<<"LV"<<"   "<<Code[pc].p<<",   "<<Code[pc].q<<endl;
	         	break;
	        case OP_LC:  
	         	cout<<"LC"<<"   "<<Code[pc].q<<endl;
	         	break;
	        case OP_LI:
	         	cout<<"LI"<<endl;
	         	break;
		 	case OP_INT:
	            cout<<"INT"<<"   "<<Code[pc].q<<endl;
	            break;
			case OP_DCT:
		 		cout<<"DCT"<<"   "<<Code[pc].q<<endl;
	            break;
			case OP_J: 
	            cout<<"J"<<"   "<<Code[pc].q<<endl;
	            break;
	        case OP_FJ:
		 		cout<<"FJ"<<"   "<<Code[pc].q<<endl;
	            break;
	        case OP_HLT:
	        	cout<<"HLT"<<endl;
	        	Exit = 1;
	            break;
	        case OP_ST:
		 		cout<<"ST"<<endl;
	            break; 
			case OP_CALL: 
	            cout<<"CALL"<<"   "<<Code[pc].p<<",   "<<Code[pc].q<<endl;         
		        break;
		    case OP_EP:
		 		cout<<"EP"<<endl;
	            break;
	        case OP_EF:
		 		cout<<"EF"<<endl;
	            break; 
	            
	        case OP_RC:
	        	cout<<"RC"<<endl;
	            break; 
			case OP_RI:
				cout<<"RI"<<endl;
	            break; 
	        case OP_WRC:
		 		cout<<"WRC"<<endl;
	            break; 
	        case OP_WRI:
		 		cout<<"WRI"<<endl;
	            break; 
	        case OP_WLN:
		 		cout<<"WLN"<<endl;
	            break; 
	            
	        case OP_ADD:
		 		cout<<"ADD"<<endl;
	            break; 
	        case OP_SUB:
		 		cout<<"SUB"<<endl;
	            break; 
	        case OP_MUL:
		 		cout<<"MUL"<<endl;
	            break; 
	        case OP_DIV:
		 		cout<<"DIV"<<endl;
	            break; 
	        case OP_NEG:
		 		cout<<"NEG"<<endl;
	            break; 
	        case OP_CV:
		 		cout<<"CV"<<endl;
	            break; 
	        
	        
	        case OP_EQ:
	        	cout<<"EQ"<<endl;
	            break; 
	        case OP_NE:
	        	cout<<"NE"<<endl;
	            break; 
	        case OP_GT:
	        	cout<<"GT"<<endl;
	            break; 
	        case OP_LT:
	        	cout<<"LT"<<endl;
	            break; 
	        case OP_GE:
	        	cout<<"GE"<<endl;
	            break; 
	        case OP_LE:
	        	cout<<"LE"<<endl;
	            break; 
	        case OP_BP:
	        	cout<<"BP"<<endl;
	        	Exit = 1;
	        	break;
	        case OP_ODD:
	        	cout<<"ODD"<<endl;
	        	break;
		} //switch
		pc++;
	}while(Exit == 0 && pc<max && pc<Code.size() );
	cout<< pc <<"    "<<endl ;
}
void init_asm(){
	Instruction i;
	i.Op = OP_J;
	Code.push_back(i);
}
void asm_LA(int depth, int offset){
	Instruction i;
	i.Op = OP_LA;
	i.p = depth;
	i.q = offset;
	Code.push_back(i);
//	cout<<"asm_LA  "<<i.Op<<"   "<<i.p<<"   "<<i.q<<endl;
}

void asm_LV(int depth, int offset){
	Instruction i;
	i.Op = OP_LV;
	i.p = depth;
	i.q = offset;
	Code.push_back(i);
}
void asm_LC(int value){
	Instruction i;
	i.Op = OP_LC;
	i.q = value;
	Code.push_back(i);
}
void asm_LI(){
	Instruction i;
	i.Op = OP_LI;
	Code.push_back(i);
}
void asm_INT(int value){
	Instruction i;
	i.Op = OP_INT;
	i.q = value;
	Code.push_back(i);
}
void asm_DCT(int value){
	Instruction i;
	i.Op = OP_DCT;
	i.q = value;
	Code.push_back(i);
}
void asm_J(int value){
	Instruction i;
	i.Op = OP_J;
	i.q = value;
	Code.push_back(i);
}
void asm_FJ(int value){
	Instruction i;
	i.Op = OP_FJ;
	i.q = value;
	Code.push_back(i);
}
void asm_HLT(){
	Instruction i;
	i.Op = OP_HLT;
	Code.push_back(i);
}

void asm_ST(){
	Instruction i;
	i.Op = OP_ST;
	Code.push_back(i);
}
void asm_CALL(int depth, int offset){
	Instruction i;
	i.Op = OP_CALL;
	i.p = depth;
	i.q = offset;
	Code.push_back(i);
}
void asm_EP(){
	Instruction i;
	i.Op = OP_EP;
	Code.push_back(i);
}
void asm_EF(){
	Instruction i;
	i.Op = OP_EF;
	Code.push_back(i);
}
void asm_RC(){
	Instruction i;
	i.Op = OP_RC;
	Code.push_back(i);
}
void asm_RI(){
	Instruction i;
	i.Op = OP_RI;
	Code.push_back(i);
}
void asm_WRC(){
	Instruction i;
	i.Op = OP_WRC;
	Code.push_back(i);
}
void asm_WRI(){
	Instruction i;
	i.Op = OP_WRI;
	Code.push_back(i);
}
void asm_WLN(){
	Instruction i;
	i.Op = OP_WLN;
	Code.push_back(i);
}
void asm_ADD(){
	Instruction i;
	i.Op = OP_ADD;
	Code.push_back(i);
}
void asm_SUB(){
	Instruction i;
	i.Op = OP_SUB;
	Code.push_back(i);
}
void asm_MUL(){
	Instruction i;
	i.Op = OP_MUL;
	Code.push_back(i);
}
void asm_DIV(){
	Instruction i;
	i.Op = OP_DIV;
	Code.push_back(i);
}
void asm_ODD(){
	Instruction i;
	i.Op = OP_ODD;
	Code.push_back(i);
}
void asm_NEG(){
	Instruction i;
	i.Op = OP_NEG;
	Code.push_back(i);
}
void asm_CV(){
	Instruction i;
	i.Op = OP_CV;
	Code.push_back(i);
}
void asm_EQ(){
	Instruction i;
	i.Op = OP_EQ;
	Code.push_back(i);
}
void asm_NE(){
	Instruction i;
	i.Op = OP_NE;
	Code.push_back(i);
}
void asm_GT(){
	Instruction i;
	i.Op = OP_GT;
	Code.push_back(i);
}
void asm_LT(){
	Instruction i;
	i.Op = OP_LT;
	Code.push_back(i);
}
void asm_GE(){
	Instruction i;
	i.Op = OP_GE;
	Code.push_back(i);
}
void asm_LE(){
	Instruction i;
	i.Op = OP_LE;
	Code.push_back(i);
}
void asm_BP(){
	Instruction i;
	i.Op = OP_BP;
	Code.push_back(i);
}

