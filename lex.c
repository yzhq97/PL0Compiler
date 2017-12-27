#include "pl0.h"

#define nxsym(P) fgetc(P)

lexelem * lex_list;
int lex_length;

const char* reserver[]={
    "const", "var", "procedure", "call",
    "begin", "end", "if", "then",
    "else", "while", "do", "read",
    "write", "odd", "repeat", "until"
}; //16

const char special_symbols[]={
    '+', '-', '*', '/',
    '(', ')', '=', ',' ,
    '.', '<', '>', ';' ,
    ':'
}; //13

int lex_ind;

void lex (FILE * ifp) {
    char ch;
    int ahead = 0; //往前读的字符数
    int comments = 0;
    
    ch = nxsym(ifp);
    while (ch != EOF) {
        if (ch==' ' || ch=='\t' || ch=='\r' || ch=='\n') {
            ch = nxsym(ifp);
            ahead= 0;
            continue;
        }
        
        //字母
        if (isalpha(ch)) {
            char buf[IDENTIFIER_LENGTH+5];
            memset(buf, 0, sizeof(buf));
            
            int i = 0;
            buf[i] = ch;
            
            i++;
            ahead= 1;
            
            while (isalpha(ch=nxsym(ifp)) || isdigit(ch)) {
                if (i > IDENTIFIER_LENGTH) {
                    printf("Identifier %s name... too long", buf);
                    while (isalpha(ch=nxsym(ifp)) || isdigit(ch));
                    break;
                }
                buf[i++] = ch;
            }
            
            //检查保留字
            int is_reserver = -1;
            for (int i = 0; i < 16; i++)
                if (strcmp(buf, reserver[i]) == 0) {
                    is_reserver = i;
                    break;
                }
            
            switch (is_reserver) {
                case 0:
                    lex_list[lex_ind].token = const_sym;
                    break;
                case 1:
                    lex_list[lex_ind].token = var_sym;
                    break;
                case 2:
                    lex_list[lex_ind].token = proc_sym;
                    break;
                case 3:
                    lex_list[lex_ind].token = call_sym;
                    break;
                case 4:
                    lex_list[lex_ind].token = begin_sym;
                    break;
                case 5:
                    lex_list[lex_ind].token = end_sym;
                    break;
                case 6:
                    lex_list[lex_ind].token = if_sym;
                    break;
                case 7:
                    lex_list[lex_ind].token = then_sym;
                    break;
                case 8:
                    lex_list[lex_ind].token = else_sym;
                    break;
                case 9:
                    lex_list[lex_ind].token = while_sym;
                    break;
                case 10:
                    lex_list[lex_ind].token = do_sym;
                    break;
                case 11:
                    lex_list[lex_ind].token = read_sym;
                    break;
                case 12:
                    lex_list[lex_ind].token = write_sym;
                    break;
                case 13:
                    lex_list[lex_ind].token = odd_sym;
                    break;
                case 14:
                    lex_list[lex_ind].token = repeat_sym;
                    break;
                case 15:
                    lex_list[lex_ind].token = until_sym;
                    break;
                    
                default:
                    lex_list[lex_ind].token = ident_sym;
                    strcpy(lex_list[lex_ind].name, buf);
                break;
            }
            lex_ind += 1;
        }
        
        //数字
        else if (isdigit(ch)) {
            int number = ch - '0';
            ahead= 1;
            int length = 1;
            
            while (isdigit(ch = nxsym(ifp))) {
                if (length > NUM_LENGTH) {
                    printf("number %d... is too long\n", number);
                    while (isdigit(ch = nxsym(ifp)));
                    break;
                }
                number = 10 * number + (ch - '0');
                length += 1;
            }
            
            lex_list[lex_ind].token = number_sym;
            lex_list[lex_ind].value = number;
            lex_ind += 1;
        }
        
        //特殊符号
        else {
            ahead= 0;
            char special_sym = -1;
            for (int i = 0; i < 13; i++)
                if (ch == special_symbols[i]) {
                    special_sym = i;
                    break;
                }
            
            switch(special_sym){
                case 0:
                    lex_list[lex_ind].token = plus_sym;
                    lex_ind += 1;
                    break;
                case 1:
                    lex_list[lex_ind].token = minus_sym;
                    lex_ind += 1;
                    break;
                case 2:
                    lex_list[lex_ind].token = mult_sym;
                    lex_ind += 1;
                    break;
                    
                //除法或注释
                case 3:
                    ch = nxsym(ifp);
                    ahead = 1;
                    if(ch == '*'){
                        comments = 1;
                        ahead = 0;
                        ch = nxsym(ifp);
                        while(comments == 1){
                            if(ch =='*'){
                                ch =nxsym(ifp);
                                if(ch =='/') comments=0;
                            } else ch =nxsym(ifp);
                        }
                    } else{
                        lex_list[lex_ind].token = slash_sym;
                        lex_ind += 1;
                    }
                    break;
                    
                case 4:
                    lex_list[lex_ind].token = lparent_sym;
                    lex_ind += 1;
                    break;
                case 5:
                    lex_list[lex_ind].token = rparent_sym;
                    lex_ind += 1;
                    break;
                case 6:
                    lex_list[lex_ind].token = eq_sym;
                    lex_ind += 1;
                    break;
                case 7:
                    lex_list[lex_ind].token = comma_sym;
                    lex_ind += 1;
                    break;
                case 8:
                    lex_list[lex_ind].token = period_sym;
                    lex_ind += 1;
                    break;
                case 9:
                    ch = nxsym(ifp);
                    ahead = 1;
                    if(ch =='>'){
                        lex_list[lex_ind].token = neq_sym;
                        ahead=0;
                    }
                    else if(ch =='='){
                        lex_list[lex_ind].token = leq_sym;
                        ahead=0;
                    }
                    else{
                        lex_list[lex_ind].token = les_sym;
                    }
                    lex_ind += 1;
                    break;
                case 10:
                    ch = nxsym(ifp);
                    ahead=1;
                    if(ch == '='){
                        lex_list[lex_ind].token = geq_sym;
                        ahead=0;
                    }
                    else{
                        lex_list[lex_ind].token = gtr_sym;
                    }
                    lex_ind += 1;
                    break;
                case 11:
                    lex_list[lex_ind].token = semicolon_sym;
                    lex_ind += 1;
                    break;
                case 12:
                    ch = nxsym(ifp);
                    if(ch == '='){
                        lex_list[lex_ind].token = becomes_sym;
                        lex_ind += 1;
                    } else {
                        printf("Invalid symbol\n");
                    }
                    break;
                default:
                    printf("Invalid symbols\n");
                    break;
            }
        }
        if (ahead == 0) ch = nxsym(ifp);
    }
    lex_length = lex_ind;
}