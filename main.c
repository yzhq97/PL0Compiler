#include "pl0.h"

int main(int argc, const char * argv[]) {
    FILE * ifp = fopen("/Users/yzhq/Code/C/YZQ-PL0/YZQ-PL0/test/testrepeat.pas", "r");
    
    int output_lex = 0;
    int output_sym = 0;
    int output_code = 0;
    
    lex_list = (lexelem *)malloc(sizeof(lexelem) * LEX_LIST_LENGTH);
    sym_table = (sym *)malloc(sizeof(sym) * SYMBOL_TABLE_SIZE);
    code = (aop *)malloc(sizeof(aop) * SYMBOL_TABLE_SIZE);
    
    lex(ifp);
    
    if (output_lex) {
        for (int i = 0; i < lex_length; i++) {
            printf("%s ", token_str[lex_list[i].token]);
            if (lex_list[i].token == ident_sym)
                printf("%s ", lex_list[i].name);
            if (lex_list[i].token == number_sym)
                printf("%d ", lex_list[i].value);
            printf("| ");
        }
        printf("\n\n");
    }
    
    parse();
    
    if (output_sym) {
        for (int i = 0; i < 3; i++)
            printf("%2d: %d %5s %3d %3d %3d\n", i,
                   sym_table[i].type, sym_table[i].name, sym_table[i].val, sym_table[i].level, sym_table[i].addr);
        printf("\n");
    }
    
    if (output_code) {
        for (int i = 0; i < code_length; i++)
            printf("%2d: %s %3d %3d\n", i, pcode_str[code[i].op], code[i].l, code[i].a);
        printf("\n");
    }
    
    free(lex_list);
    free(sym_table);
    
    stack = (int *)malloc(sizeof(int) * STACK_SIZE);
    
    interpret();
    return 0;
}
