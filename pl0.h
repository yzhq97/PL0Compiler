#ifndef PL0
#define PL0

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define SYMBOL_TABLE_SIZE 1000
#define CODE_SIZE 1000
#define STACK_SIZE 2000
#define LEX_LIST_LENGTH 5000

#define IDENTIFIER_LENGTH 20
#define NUM_LENGTH 8
#define MAX_ADDR 2048
#define MAX_LEXI_LEVELS 3

extern int lexoutput;
extern int codegenoutput;
extern int vmoutput;

extern int errorCount;


//词法分析数据结构
typedef enum {
    illegal_sym,
    nul_sym, ident_sym, number_sym, plus_sym, minus_sym,
    mult_sym,  slash_sym, odd_sym, eq_sym, neq_sym, les_sym, leq_sym,
    gtr_sym, geq_sym, lparent_sym, rparent_sym, comma_sym, semicolon_sym,
    period_sym, becomes_sym, begin_sym, end_sym, if_sym, then_sym,
    while_sym, do_sym, call_sym, const_sym, var_sym, proc_sym, write_sym,
    read_sym , else_sym, repeat_sym, until_sym
} token_t;

extern const char * token_str[36];

typedef struct {
    token_t token;
    int value;
    char name[IDENTIFIER_LENGTH];
} lexelem;

extern lexelem * lex_list;
extern int lex_length;

//符号表数据结构
typedef enum {
    const_type = 1, var_type, proc_type
} symtype_t;

typedef struct {
    symtype_t type;
    char name[IDENTIFIER_LENGTH];
    int val;
    int level;
    int addr;
} sym;

extern sym * sym_table;
extern int sym_length;
/* 常数保存type, name, value
   变量保存type, name, level, addr
   过程保存type, name, level, addr */

//pcode数据结构
typedef enum {
    illegal_op,
    lit_op, opr_op, lod_op, sto_op, cal_op,
    int_op, jmp_op, jpc_op, red_op, wrt_op
} pcode_t;

typedef enum {
    ret_op, neg_op, add_op, sub_op, mul_op,
    div_op, odd_op, mod_op, eql_op, neq_op,
    lss_op, leq_op, gtr_op, geq_op
} opr_t;

extern const char * pcode_str[11];

typedef struct {
    pcode_t op;
    int l;
    int a;
} aop;

extern aop * code;
extern int code_length;

extern int * stack;

//主要的函数
void compile(void);

void lex(FILE * ifp);
void parse();
void interpret();

#endif