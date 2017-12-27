#include "pl0.h"

void print_stack_frame (int SP, int BP);
void execute_cycle (aop * instru, int * sp, int * bp, int * pc);
void OPR (int * sp, int * bp, int * pc, aop * instru);
int base (int * bp, aop * instru);

int * stack;

void interpret (void) {
    aop * instru;
    
    int SP = 0;
    int BP = 1;
    int PC = 0;
    
    while (BP!=0) {
        //Prints out the PC, OP, L, and M. This is also the "Fetch Cycle"
        instru = &code[PC];
        printf("instruction %2d | %3s %4d %4d\n", PC, pcode_str[instru->op], instru->l, instru->a);
        PC++;
        
        //Sends to giant switch function, mimicking the execute cycle
        execute_cycle(instru, &SP, &BP, &PC);
        printf("registers:  PC: %3d  BP: %3d  SP: %3d\n", PC, BP, SP);

        //Calls function that prints out each stack frame and its values, separated by |
        printf("stack: ");
        print_stack_frame(SP, BP);
        
        printf("\n\n");
    }
}

void print_stack_frame (int SP, int BP) {
    //Base Case #1: if BP is 0, the program has finished. No stack frames are left to print out
    if (BP == 0) return;
    //Base Case #2: if BP is 1, then it is in the main stack frame, and we print out the stack from BP to SP, with BP pointing to the bottom of the main stack frame, and SP pointing to the top of the stack
    else if (BP == 1) {
        for(int i = 1; i <= SP; i++)
            printf("%d ",stack[i]);
        return;
    }
    //Recursive Case: Prints out each new stack frame, separating them with |
    else {
        print_stack_frame(BP-1, stack[BP+2]);
        
        //Covers one case, where CAL instruction is just called, meaning a new Stack Frame is created, but SP is still less than BP
        if (SP < BP) {
            printf("| ");
            for (int i = 0; i < 4; i++)
                printf("%d ", stack[BP+i]);
        }
        //For SP being greater than BP, aka most cases
        else {
            printf("| ");
            for (int i = BP; i <= SP; i++)
                printf("%d ", stack[i]);
        }
        return;
    }
}

void execute_cycle (aop * instru, int* sp, int* bp, int* pc) {
    switch(instru->op) {
        case lit_op: //LIT
            *sp = *sp+1;
            stack[*sp] = instru->a;
            break;
        case opr_op: //OPR function
            OPR(sp, bp, pc, instru);
            break;
        case lod_op: //LOD
            *sp = *sp + 1;
            stack[*sp] = stack[base(bp, instru) + instru->a];
            break;
        case sto_op: //STO
            stack[base(bp, instru) + instru->a] = stack[*sp];
            *sp = *sp - 1;
            break;
        case cal_op: //CAL
            stack[*sp+1] = 0; //space to return value
            stack[*sp+2] = base(bp, instru); //static link (SL)
            stack[*sp+3] = *bp; //dynamic link (DL)
            stack[*sp+4] = *pc; //return address (RA)
            *bp = *sp+1;
            *pc = instru->a;
            break;
        case int_op: //INT
            *sp = *sp + instru->a;
            break;
        case jmp_op: //JMP
            *pc = instru->a;
            break;
        case jpc_op: //JPC
            if (stack[*sp] == 0)
                *pc = instru->a;
            *sp = *sp - 1;
            break;
        case red_op: //RED
            *sp = *sp+1;
            printf("PROGRAM INPUT: ");
            scanf("%d", &stack[*sp]);
            break;
        case wrt_op: //WRT
            printf("PROGRAM OUTPUT: %d\n", stack[*sp]);
            *sp = *sp - 1;
            break;
        default:
            printf("Illegal OPR!\n");
    }
}

void OPR (int *sp, int* bp, int *pc, aop * instru) {
    
    switch (instru->a) {
        case ret_op:
            *sp = *bp-1;
            *pc = stack[*sp+4];
            *bp = stack[*sp+3];
            break;
        case neg_op:
            stack[*sp] = -stack[*sp];
            break;
        case add_op: //ADD
            *sp=*sp-1;
            stack[*sp] = stack[*sp] + stack[*sp+1];
            break;
        case sub_op: //SUB
            *sp = *sp-1;
            stack[*sp] = stack[*sp] - stack[*sp+1];
            break;
        case mul_op: //MUL
            *sp = *sp-1;
            stack[*sp] = stack[*sp] * stack[*sp+1];
            break;
        case div_op: //DIV
            *sp = *sp-1;
            stack[*sp] = stack[*sp] / stack[*sp+1];
            break;
        case odd_op: //ODD
            stack[*sp] = stack[*sp] % 2;
            break;
        case mod_op: //MOD
            *sp = *sp-1;
            stack[*sp] = stack[*sp] % stack[*sp+1];
            break;
        case eql_op: //EQL
            *sp = *sp-1;
            stack[*sp] = stack[*sp] == stack[*sp+1];
            break;
        case neq_op: //NEQ
            *sp = *sp-1;
            stack[*sp] = stack[*sp] != stack[*sp+1];
            break;
        case lss_op: //LSS
            *sp = *sp-1;
            stack[*sp] = stack[*sp] < stack[*sp+1];
            break;
        case leq_op: //LEQ
            *sp = *sp-1;
            stack[*sp] = stack[*sp] <= stack[*sp+1];
            break;
        case gtr_op: //GTR
            *sp = *sp-1;
            stack[*sp] = stack[*sp] > stack[*sp+1];
            break;
        case geq_op: //GEQ
            *sp = *sp-1;
            stack[*sp]=stack[*sp] >= stack[*sp+1];
            break;
    }
}

//Base function: function used to find base L levels down
//l stans for L in the instruction format, passed through the irStruct
int base(int * bp, aop * instru) {
    int l = instru->l;
    int b1; //find base L levels down
    b1 = *bp;
    while (l>0) {
        b1=stack[b1+1];
        l--;
    }
    return b1;
}
