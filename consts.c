#include "pl0.h"

const char * token_str[36] = {
    "illegal_sym",
    "nul_sym", "ident_sym", "number_sym", "plus_sym", "minus_sym",
    "mult_sym",  "slash_sym", "odd_sym", "eq_sym", "neq_sym", "les_sym", "leq_sym",
    "gtr_sym", "geq_sym", "lparent_sym", "rparent_sym", "comma_sym", "semicolon_sym",
    "period_sym", "becomes_sym", "begin_sym", "end_sym", "if_sym", "then_sym",
    "while_sym", "do_sym", "call_sym", "const_sym", "var_sym", "proc_sym", "write_sym",
    "read_sym" , "else_sym", "repeat_sym", "until_sym"
};

const char * pcode_str[11] = {
    "illegal",
    "lit", "opr", "lod", "sto", "cal",
    "int", "jmp", "jpc", "red", "wrt"
};