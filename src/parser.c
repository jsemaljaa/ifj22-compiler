/*
 * IFJ2022/project/parser.c 
 * 
 * @brief Syntactic and semantic analysis implementation
 * 
 * @author Vinogradova Alina <xvinog00@vutbr.cz>
 * @author Mazurava Maryia <xmazur08@vutbr.cz>
 */


#include "parser.h"
#include "scanner.h"
#include "error.h"

htable globalSymt;
htable localSymt;
token_t token;
int ret;
int code;

#define GET_TOKEN()                 \
    ret = get_next_token(&token);   \

#define CHECK_KEYW(KEYW)                                                        \
    if(!(token.type == TOKEN_KEY_W && token.attribute.keyword == (KEYW))) {     \
        return false;                                                           \
    }

#define CHECK_TYPE(TYPE)                        \
    if(!(token.type == TYPE)) return false;     \

#define GET_TOKEN_CHECK_KEYW(KEYW)      \
    do {                                \
        GET_TOKEN();                    \
        CHECK_KEYW(KEYW);               \
    } while(true)                       \

#define CHECK_TOKEN(expr, code)         \
    if(!(expr)) return code;            \

#define GET_AND_CHECK_TOKEN(expr, code) \
    GET_TOKEN();                        \
    CHECK_TOKEN(expr, code);            \

#define CHECK_ERROR(code)               \
    if(code != 0) return code;          \


static void printf_symtable_func_debug(ht_item_t* item){
    printf("--- HT_ITEM FUNCTION DATA ---\n");
    printf("Function key is: %s\n", item->key);
    printf("Is function defined? %s\n", (item->data.func->defined) ? "Yes" : "No");
    printf("Function arguments: %s\n", item->data.func->argv.str);
    printf("Function return type: %s\n", item->data.func->ret.str);
    printf("---END HT_ITEM FUNCTION DATA ---\n");
}

static void printf_token_debug(token_t token){
    printf("--- BEGIN CURRENT TOKEN INFO ---\n");
    printf("Token: %s \n", list[token.type]);
    switch (token.type){
        case TOKEN_TYPE_INT:
            printf("Value: %d \n", token.attribute.integer);
            break;
        
        case TOKEN_TYPE_FLOAT:
            printf("Value: %f \n", token.attribute.decimal);
            break;

        case TOKEN_KEY_W:
            printf("Value: %d \n", token.attribute.keyword);
            break; 
        
        case TOKEN_ID:
            printf("Value: %s \n", token.attribute.string->str);
            break;
        case TOKEN_TYPE_STRING:
            printf("Value: %s \n", token.attribute.string->str);
            break;
        
        default:
            break;
    }
    printf("--- END CURRENT TOKEN INFO ---\n");
}

static void printf_return_code_debug(int code, char* func){
    printf("\n--- START RETURN DEBUG ---\n");
    printf("Returning [%d] from %s\n", code, func);
    printf("--- END RETURN DEBUG ---\n");
}

int symt_add_internal_functions(){
    string_t internals[INTERNALS_COUNT] = {
        {.length=5, .allocSize=0, .str="reads"},
        {.length=5, .allocSize=0, .str="readi"},
        {.length=5, .allocSize=0, .str="readf"},
        {.length=5, .allocSize=0, .str="write"},
        {.length=8, .allocSize=0, .str="floatval"},
        {.length=6, .allocSize=0, .str="intval"},
        {.length=6, .allocSize=0, .str="strval"},
        {.length=6, .allocSize=0, .str="strlen"},
        {.length=9, .allocSize=0, .str="substring"},
        {.length=3, .allocSize=3, .str="ord"},
        {.length=3, .allocSize=0, .str="chr"}
    };

    const char *args[INTERNALS_COUNT] = {"", "", "", ".T", "T", "T", "T", "S", "SII", "S", "I"}; 
    const char *ret[INTERNALS_COUNT] = {"?S", "?I", "?F", "V", "F", "I", "S", "I", "?S", "I", "S"};

    
    int i = 0;
    while(i < INTERNALS_COUNT){
        ht_item_t* item = symt_add_symb(&globalSymt, &(internals[i]));
        
        if(item == NULL) {
            return INTERNAL_ERROR;
        }
        
        
        item->data.func = malloc(sizeof(symt_func_t));
        if(item->data.func == NULL){
            return ALLOCATION_ERROR;
        }
        
        item->type = func;
        item->data.func->defined = true;

        if(str_init(&item->data.func->argv) == 1 || str_init(&item->data.func->ret) == 1){
            return INTERNAL_ERROR;
        }
        
        
        int k = 0;
        while(args[i][k] != 0){
            if(str_add_char(&item->data.func->argv, args[i][k]) == 1){
                return INTERNAL_ERROR;
            }
            k++;
        }

        k = 0;
        while(ret[i][k] != 0){
            if(str_add_char(&item->data.func->ret, ret[i][k]) == 1){
                return INTERNAL_ERROR;
            }
            k++;
        }
        // printf_symtable_func_debug(item);
        i++;
    }

    return NO_ERRORS;
}

int parse() {
    code = symt_add_internal_functions();
    CHECK_ERROR(code);

    get_next_token(&token);     
    if(token.type == TOKEN_END_OF_FILE) return true;
    
    code = program();

    printf("exit code is %d\n", code);
    exit_error(code);     
}


int program(){
    switch (token.type){
    case TOKEN_PROLOG:
        if(prolog() != 0) return SYNTAX_ERROR;

        return list_of_statements();
    default:
        return false;
        break;
    }    
}

int prolog(){

    GET_AND_CHECK_TOKEN(token.type == TOKEN_ID, SYNTAX_ERROR);
    if(str_cmp_const_str(token.attribute.string, "declare")) return SYNTAX_ERROR;
    
    GET_AND_CHECK_TOKEN(token.type == TOKEN_LEFT_PAR, SYNTAX_ERROR);

    GET_AND_CHECK_TOKEN(token.type == TOKEN_ID, SYNTAX_ERROR);
    if(str_cmp_const_str(token.attribute.string, "strict_types")) return SYNTAX_ERROR;

    GET_AND_CHECK_TOKEN(token.type == TOKEN_ASSIGN, SYNTAX_ERROR);
        
    GET_AND_CHECK_TOKEN(token.type == TOKEN_TYPE_INT, SYNTAX_ERROR);
    if(token.attribute.integer != 1) return SYNTAX_ERROR; 
    
    GET_AND_CHECK_TOKEN(token.type == TOKEN_RIGHT_PAR, SYNTAX_ERROR);
    GET_AND_CHECK_TOKEN(token.type == TOKEN_SEMICOLON, SYNTAX_ERROR);

    return 0;
}

//3. <list_of_statements> -> EOF
//4. <list_of_statements> -> <statement> <list_of_statements>
int list_of_statements(){
    while(token.type != TOKEN_END_OF_FILE){
        GET_TOKEN();
        if(token.type == TOKEN_END_OF_FILE) return NO_ERRORS;
        switch (token.type){
            case TOKEN_ID:
                code = statement();
                    
                CHECK_ERROR(code);
                
                code = list_of_statements(); 
                    
                CHECK_ERROR(code);
                
                return NO_ERRORS;
            case TOKEN_KEY_W:
                if(token.attribute.keyword == K_IF || token.attribute.keyword == K_RETURN || token.attribute.keyword == K_WHILE){
                    code = statement();
                        
                    CHECK_ERROR(code);
                } else return SEM_OTHER_ERROR;
        }
    }
}

//5. <statement> -> <function_definition>
//6. <statement> -> <variable_definition>
//7. <statement> -> <function_call>
//8. <statement> -> if ( <expression> ) { <list_of_statements> } else { <list_of_statements> }
//9. <statement> -> while ( <expression> ) { <list_of_statements> }
//10. <statement> -> return <return_expressions>;
int statement(){
    switch (token.type){
    case TOKEN_ID:
        if(token.attribute.string->str[0] == '$'){
            code = variable_definition();
                
            CHECK_ERROR(code);
        } else {
            code = function_call(); // internal function OR user function call
                
            CHECK_ERROR(code);
        }
        break;
    case TOKEN_KEY_W:
        printf_token_debug(token);
        if(token.attribute.keyword == K_FUNCTION){
            code = function_definition(); 
                
            CHECK_ERROR(code);
        } else if(token.attribute.keyword == K_IF || token.attribute.keyword == K_WHILE){
            GET_AND_CHECK_TOKEN(token.type == TOKEN_LEFT_PAR, SYNTAX_ERROR);
            // if(!expression_check()) return false;
            
            GET_AND_CHECK_TOKEN(token.type == TOKEN_RIGHT_PAR, SYNTAX_ERROR);
            GET_AND_CHECK_TOKEN(token.type == TOKEN_LEFT_BR, SYNTAX_ERROR);
            
            code = list_of_statements();
                
            CHECK_ERROR(code);
            
            GET_AND_CHECK_TOKEN(token.type == TOKEN_RIGHT_BR, SYNTAX_ERROR);

            GET_AND_CHECK_TOKEN(token.type == TOKEN_KEY_W, SYNTAX_ERROR);
            if(str_cmp_const_str(token.attribute.string, "else")) 
                return SEM_STMT_FUNC_ERROR;

            GET_AND_CHECK_TOKEN(token.type == TOKEN_LEFT_BR, SYNTAX_ERROR);
            
            code = list_of_statements();
                
            CHECK_ERROR(code);
            
            GET_AND_CHECK_TOKEN(token.type == TOKEN_RIGHT_BR, SYNTAX_ERROR);

        } else if(!str_cmp_const_str(token.attribute.string, "return")){

        }
        break;
    }
}


// 13. <function_definition> -> function ID( <list_of_parameters> ) : <list_of_datatypes_ret> { <list_of_statements> }
int function_definition(){
    GET_AND_CHECK_TOKEN(token.type == TOKEN_ID, SYNTAX_ERROR);
    // control if ID isn't a keyword, reserved word, etc.
    symt_add_symb(&globalSymt, token.attribute.string);

    ht_item_t* item = symt_search(&globalSymt, token.attribute.string->str);
    if(item == NULL) return INTERNAL_ERROR; // something went wrong with htable

    item->type = func;
    item->data.func = malloc(sizeof(symt_func_t));
    if(item->data.func == NULL){
        return ALLOCATION_ERROR;
    }

    if(str_init(&item->data.func->argv) == 1 || str_init(&item->data.func->ret) == 1){
        return INTERNAL_ERROR;
    }

    GET_AND_CHECK_TOKEN(token.type == TOKEN_RIGHT_PAR, SYNTAX_ERROR);
    code = list_of_parameters(item);
        
    CHECK_ERROR(code);

    GET_AND_CHECK_TOKEN(token.type == TOKEN_RIGHT_BR, SYNTAX_ERROR);
    GET_AND_CHECK_TOKEN(token.type == TOKEN_COLON, SYNTAX_ERROR);

    code = list_of_datatypes_ret(item);
        
    CHECK_ERROR(code);



    item->data.func->defined = true;
}

/*
24. <list_of_parameters> -> ε
25. <list_of_parameters> -> <parameter> <list_of_parameters_n>
*/
int list_of_parameters(ht_item_t* item){

    const char *emptyString[1] = {""};
    GET_TOKEN();
    if(token.type == TOKEN_RIGHT_PAR){
        str_add_char(&item->data.func->argv, emptyString[0][0]);
    } else {
        code = parameter(item);
            
        CHECK_ERROR(code);

        code = list_of_call_parameters_n(item);
            
        CHECK_ERROR(code);
    }
}

//26. <parameter> -> <list_of_datatypes> <variable>
int parameter(ht_item_t* item){
    GET_AND_CHECK_TOKEN(token.type == TOKEN_KEY_W, SYNTAX_ERROR);
    if (token.attribute.keyword == K_FLOAT || token.attribute.keyword == K_INT || token.attribute.keyword == K_STRING){
        code = variable();
            
        CHECK_ERROR(code);
    }
}

/*
33. <list_of_parameters_n> -> ε
34. <list_of_parameters_n> -> , <parameter> <list_of_parameters_n>
*/
int list_of_parameters_n(ht_item_t* item){

}


/*
35. <list_of_datatypes_ret> -> void
36. <list_of_datatypes_ret> -> <list_of_datatypes>
*/
int list_of_datatypes_ret(ht_item_t* item){
    GET_TOKEN();
    if(token.type == TOKEN_KEY_W){
        if(token.attribute.keyword == K_FLOAT){
            str_add_char(&item->data.func->ret, retDatatypes[0][1]);
        } else if(token.attribute.keyword == K_INT){
            str_add_char(&item->data.func->ret, retDatatypes[0][0]);
        } else if(token.attribute.keyword == K_STRING){
            str_add_char(&item->data.func->ret, retDatatypes[0][2]);
        } else if(token.attribute.keyword == K_VOID){
            str_add_char(&item->data.func->ret, retDatatypes[0][3]);
        }
    }
}


// 14. <variable_definition> -> <variable> = <var_def_expr>
int variable_definition(){
    
}

// 15. <var_def_expr> -> <function_call>
// 16. <var_def_expr> -> <expression>;
int var_def_expr(){
    GET_TOKEN();
    
    if(token.type == TOKEN_ID){
        if(token.attribute.string->str[0] == '$'){
            ht_item_t* item = symt_search(&globalSymt, token.attribute.string->str);
            if(item == NULL) return SEM_VAR_ERROR;

            // call an expression check
        } else {
            code = function_call();
                
            CHECK_ERROR(code);
        }
    }

    // if(token.type == TOKEN_ID){
    //     ht_item_t* item = symt_search(&(pData->globalSymt), token.attribute.string->str);
    //     if(item == NULL){
    //         return SEM_VAR_ERROR;
    //     } else{
    //         if(item->type == func){
    //             code = function_call(pData);
    //             CHECK_ERROR(code);    
    //         } else if(item->type == var){
    //             // call an expression_check
    //         }
    //     }
    // } else {
    //     // call an expression_check
    //     // if expression is ok, then save/rewrite data in symtable
    //     // TODO: in expression_check control whether ; is here or not
    // }   
}

//17. <function_call> -> ID( <list_of_call_parameters> );
int function_call(){
    ht_item_t* item = symt_search(&globalSymt, token.attribute.string->str);
    if(item == NULL) {
        return SEM_DEF_FUNC_ERROR;
    } else if(item->type == func){
        GET_AND_CHECK_TOKEN(token.type == TOKEN_LEFT_PAR, SYNTAX_ERROR);
        code = list_of_call_parameters(item);
        //  
        CHECK_ERROR(code);
    } else return INTERNAL_ERROR;
    return NO_ERRORS;
}

//18. <list_of_call_parameters> -> ε
//19. <list_of_call_parameters> -> <call_parameter> <list_of_call_parameters_n>
int list_of_call_parameters(ht_item_t* item){
    GET_TOKEN();
    if(token.type == TOKEN_RIGHT_PAR){
        if(!str_cmp_const_str(&item->data.func->argv, "")){
            GET_AND_CHECK_TOKEN(token.type == TOKEN_SEMICOLON, SYNTAX_ERROR);
            return NO_ERRORS;
            // generate a function call
        } else return SEM_TYPE_ERROR;
    } else if (token.type == TOKEN_ID || token.type == TOKEN_TYPE_STRING){
        code = call_parameter(item);
        CHECK_ERROR(code);
        
        code = list_of_call_parameters_n(item);
        CHECK_ERROR(code);
        // generate a function call
    }
}

/*
22. <list_of_call_parameters_n> -> , <call_parameter> <list_of_call_parameters_n>
23. <list_of_call_parameters_n> -> ε 
*/
int list_of_call_parameters_n(ht_item_t* item){
    GET_TOKEN();
    if(token.type == TOKEN_RIGHT_PAR){
        return NO_ERRORS;
    } else if (token.type == TOKEN_COMMA){
        GET_TOKEN();
        code = call_parameter(item);
        CHECK_ERROR(code);
        
        code = list_of_call_parameters_n(item);
        CHECK_ERROR(code);
        // generate a function call
    }
}

//20. <call_parameter> -> <variable>
//21. <call_parameter> -> "string" // ?????
int call_parameter(ht_item_t* item){
    if(token.type == TOKEN_TYPE_STRING){
        return NO_ERRORS;
    } else if(token.type == TOKEN_ID){
        code = variable();
            
        CHECK_ERROR(code);
    }   
    
    // string check
}



//32. <variable> -> $ID
int variable(){
    // assuming that in this function we have token which is before TOKEN_TYPE_ID
    string_t varName;
    
    string_t keywList[KEYW_COUNT] = {
        {.length=4, .allocSize=0, .str="else"},
        {.length=5, .allocSize=0, .str="float"},
        {.length=8, .allocSize=0, .str="function"},
        {.length=2, .allocSize=0, .str="if"},
        {.length=3, .allocSize=0, .str="int"},
        {.length=4, .allocSize=0, .str="null"},
        {.length=6, .allocSize=0, .str="return"},
        {.length=6, .allocSize=0, .str="string"},
        {.length=4, .allocSize=0, .str="void"},
        {.length=5, .allocSize=0, .str="while"},
        {.length=3, .allocSize=0, .str="php"}
    };

    if(token.type != TOKEN_ID) GET_AND_CHECK_TOKEN(token.type == TOKEN_ID, SYNTAX_ERROR);

    if(token.attribute.string->str[0] == '$'){
        int i = 1;
        while(token.attribute.string->str != NULL){
            str_add_char(&varName, token.attribute.string->str[i++]);
        }

        i = 0;
        while(i < KEYW_COUNT){
            if(str_cmp_string(&varName, &keywList[i++]) == 0)
                return SEM_OTHER_ERROR;
        }
    } else {
        return SYNTAX_ERROR;
    }
        
    return NO_ERRORS;
}