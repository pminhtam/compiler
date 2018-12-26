#include<vector>
#include<iostream>

using namespace std;

int b = 0;
int base(int L) {
  int c = b;
  while (L > 0) {
    c = Stack[c + 3];
    L --;
  }
  return c;
}


void intepreter(vector<Instruction> Code){
  int Exit = 0;
  int pc = 0;
  int t = 0;
  do {
//  	cout<<"lenh   "<< pc <<endl;
     switch (Code[pc].Op) {
    	case OP_LA:   t ++;
        	Stack[t] = base(Code[pc].p) + Code[pc].q;                
        	break;
     	case OP_LV:   t ++;
         	Stack[t] = Stack[base(Code[pc].p) + Code[pc].q];
         	break;
        case OP_LC:   t ++;
         	Stack[t] = Code[pc].q;
         	break;
        case OP_LI:  
         	Stack[t] = Stack[Stack[t]];
         	break;
	 	case OP_INT:
            t += Code[pc].q;
            break;
		case OP_DCT:
	 		t -= Code[pc].q;
            break;
		case OP_J: 
            pc = Code[pc].q - 1;
            break;
        case OP_FJ:
	 		if(Stack[t]==0){
	 			pc = Code[pc].q - 1;
			 }
			 t -= 1;
            break;
        case OP_HLT:
            break;
        case OP_ST:
	 		Stack[Stack[t-1]] = Stack[t];
	 		t -= 2;
            break; 
		case OP_CALL: 
            Stack[t+2] = b;                 // Dynamic Link
            Stack[t+3] = pc;                // Return Address
            Stack[t+4] = base(Code[pc].p);  // Static Link
            b = t + 1;                      // Base & Result
            pc = Code[pc].q - 1;            
	        break;
	    case OP_EP:
	 		t = b-1;
	 		pc = Stack[b+2];
	 		b = Stack[b+1];
            break;
        case OP_EF:
	 		t = b;
	 		pc = Stack[b+2];
	 		b = Stack[b+1];
            break; 
            
        case OP_RC:
        	char temp;
        	cin >>temp;
	 		Stack[Stack[t]] = temp;
	 		t -=1;
            break; 
		case OP_RI:
			int temp2;
			cin>>temp2;
	 		Stack[Stack[t]] = temp2;
	 		t -=1;
            break; 
        case OP_WRC:
	 		char temp3;
	 		temp3 = Stack[t];
	 		cout<<temp3;
	 		t -=1;
            break; 
        case OP_WRI:
        	cout<<Stack[t]<<"  ";
	 		t -=1;
            break; 
        case OP_WLN:
	 		cout<<endl;
            break; 
            
        case OP_ADD:
	 		t -= 1;
	 		Stack[t] = Stack[t] + Stack[t+1];
            break; 
        case OP_SUB:
	 		t -= 1;
	 		Stack[t] = Stack[t] - Stack[t+1];
            break; 
        case OP_MUL:
	 		t -= 1;
	 		Stack[t] = Stack[t] * Stack[t+1];
            break; 
        case OP_DIV:
	 		t -= 1;
	 		Stack[t] = Stack[t] / Stack[t+1];
            break; 
        case OP_NEG:
	 		Stack[t] = -Stack[t];
            break; 
        case OP_CV:
	 		Stack[t+1] = Stack[t];
	 		t += 1;
            break; 
        
        
        case OP_EQ:
        	t -= 1;
	 		if (Stack[t] == Stack[t+1]){
	 			Stack[t] = 1;
			 }
			 else{
			 	Stack[t] = 0;
			 }
            break; 
        case OP_NE:
        	t -= 1;
	 		if (Stack[t] != Stack[t+1]){
	 			Stack[t] = 1;
			 }
			 else{
			 	Stack[t] = 0;
			 }
            break; 
        case OP_GT:
        	t -= 1;
	 		if (Stack[t] > Stack[t+1]){
	 			Stack[t] = 1;
			 }
			 else{
			 	Stack[t] = 0;
			 }
            break; 
        case OP_LT:
        	t -= 1;
	 		if (Stack[t] < Stack[t+1]){
	 			Stack[t] = 1;
			 }
			 else{
			 	Stack[t] = 0;
			 }
            break; 
        case OP_GE:
        	t -= 1;
	 		if (Stack[t] >= Stack[t+1]){
	 			Stack[t] = 1;
			 }
			 else{
			 	Stack[t] = 0;
			 }
            break; 
        case OP_LE:
        	t -= 1;
	 		if (Stack[t] <= Stack[t+1]){
	 			Stack[t] = 1;
			 }
			 else{
			 	Stack[t] = 0;
			 }
            break; 
        case OP_ODD:
        	if(Stack[t]%2==0){
        		Stack[t] = 1;
			}
			else{
				Stack[t] = 0;
			}
			break;
        case OP_BP:
        	Exit = 1;
	} //switch
	pc++;
	}while(Exit == 0);
}//intepreter

