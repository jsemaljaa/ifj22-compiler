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
bool inWhile;
bool inIf;
bool inFunctionDefinition;
bool inFunctionCall;

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
    printf("\t- Is function defined? %s\n", (item->data.func->defined) ? "Yes" : "No");
    printf("\t- Arguments: %s\n", item->data.func->argv.str);
    printf("\t- Return type: %s\n", item->data.func->ret.str);
    printf("---END HT_ITEM FUNCTION DATA ---\n");
}

static void printf_symtable_var_debug(ht_item_t* item){
    printf("--- HT_ITEM VARIABLE DATA ---\n");
    printf("- Variable key is %s\n", item->key);
    printf("\t- Datatype: %d\n", item->data.var->type);
    printf("\t- Attributes: \n");
    printf("\t\t- integer: %d\n", item->data.var->attr->integer);
    printf("\t\t- decimal: %f\n", item->data.var->attr->decimal);
    printf("\t\t- keyword: %d\n", item->data.var->attr->keyword);
    printf("\t\t- string: %s\n", item->data.var->attr->string->str);
    printf("--- END HT_ITEM VARIABLE DATA ---\n");

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

    const char *args[INTERNALS_COUNT] = {"", "", "", ".T", "T", "T", "T", "S", "jh", "S", "I"}; 
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
        i++;
    }

    return NO_ERRORS;
}

int parse() {
    symt_init(&globalSymt);
    code = symt_add_internal_functions();
    CHECK_ERROR(code);

    get_next_token(&token);     
    if(token.type == TOKEN_END_OF_FILE) return true; // empty file
    
    code = program();

    exit_error(code);     
}


int program(){
    switch (token.type){
        case TOKEN_PROLOG:
            code = prolog();
            CHECK_ERROR(code);

            code = list_of_statements();
            // for some reason here list_of_statements returns code=2 in case of TOKEN_END_OF_FILE
            // even tho if we are at this line and token is END_OF_FILE
            // means that program is OK 
            // code = 0;
            CHECK_ERROR(code);

            return NO_ERRORS;
        
        default:
            return SYNTAX_ERROR;
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

    return NO_ERRORS;
}

//3. <list_of_statements> -> EOF
//4. <list_of_statements> -> <statement> <list_of_statements>
int list_of_statements(){
    while(true){
        GET_TOKEN();
        printf_token_debug(token);
        if(token.type == TOKEN_END_OF_FILE) {
            exit_error(NO_ERRORS);
        }

        code = statement();
        CHECK_ERROR(code);

        // return NO_ERRORS;

        // switch (token.type){
        //     case TOKEN_ID:
        //         code = statement();
        //         CHECK_ERROR(code);
                
        //         code = list_of_statements();
        //         CHECK_ERROR(code);
        //     case TOKEN_KEY_W:
        //         if(token.attribute.keyword == K_IF || token.attribute.keyword == K_RETURN || token.attribute.keyword == K_WHILE){
        //             code = statement();
        //             CHECK_ERROR(code);
        //         } else if(inIf && token.attribute.keyword == K_ELSE){
        //             code = statement();
        //             CHECK_ERROR(code);
        //         } else if(token.attribute.keyword == K_FUNCTION){
        //             code = statement();
        //             CHECK_ERROR(code);
        //         }
        //         else{
        //             // printf("\033[0;31m");
        //             // printf("ERROR TOKEN (list_of_statements):\n");
        //             // printf("inIf value is %s\n", inIf ? "True" : "False");
        //             // printf_token_debug(token);
        //             // printf("\033[0m");
        //             return SEM_OTHER_ERROR;
        //         }
        // }
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
    case TOKEN_KEY_W:
        if(token.attribute.keyword == K_FUNCTION){
            // we can't have function definition inside of an other function definition
            code = function_definition(); 
            CHECK_ERROR(code);
        } else if(token.attribute.keyword == K_IF){
            code = inside_if();
            CHECK_ERROR(code);  
        } else if(token.attribute.keyword == K_WHILE){
            code = inside_while();
            CHECK_ERROR(code);
        } else if(token.attribute.keyword == K_RETURN){}
    }

    if(token.type == TOKEN_END_OF_FILE) {
        return NO_ERRORS;
    }

    code = list_of_statements();
    CHECK_ERROR(code);

}

int inside_if(){
    GET_AND_CHECK_TOKEN(token.type == TOKEN_LEFT_PAR, SYNTAX_ERROR);
    // here we should collect an expression inside of a IF statement, then check if it's okay and after check for the TOKEN_RIGHT_PAR
    // if(!expression_check()) return false;
    while(token.type != TOKEN_RIGHT_PAR) GET_TOKEN();
    // printf("------ token inside if:\n");
    GET_AND_CHECK_TOKEN(token.type == TOKEN_LEFT_BR, SYNTAX_ERROR);
    
    inIf = true;
    code = list_of_statements();

    CHECK_ERROR(code);

    GET_AND_CHECK_TOKEN(token.type == TOKEN_RIGHT_BR, SYNTAX_ERROR);
    GET_AND_CHECK_TOKEN(token.type == TOKEN_KEY_W, SYNTAX_ERROR);

    if(token.attribute.keyword != K_ELSE) return SEM_STMT_FUNC_ERROR;

    GET_AND_CHECK_TOKEN(token.type == TOKEN_LEFT_BR, SYNTAX_ERROR);

    code = list_of_statements();

    CHECK_ERROR(code);

    GET_AND_CHECK_TOKEN(token.type == TOKEN_RIGHT_BR, SYNTAX_ERROR);

    inIf = false;
    return NO_ERRORS;
}

int inside_while(){
    GET_AND_CHECK_TOKEN(token.type == TOKEN_LEFT_PAR, SYNTAX_ERROR);
    // here we should collect an expression inside of a WHILE statement, then check if it's okay and after check for the TOKEN_RIGHT_PAR
    // if(!expression_check()) return false;
    while(token.type != TOKEN_RIGHT_PAR) GET_TOKEN();

    GET_AND_CHECK_TOKEN(token.type == TOKEN_LEFT_BR, SYNTAX_ERROR);
    inWhile = true;

    code = list_of_statements();
    CHECK_ERROR(code);

    GET_AND_CHECK_TOKEN(token.type == TOKEN_RIGHT_BR, SYNTAX_ERROR);

    inWhile = false;
    return NO_ERRORS;
}


// 13. <function_definition> -> function ID( <list_of_parameters> ) : <list_of_datatypes_ret> { <list_of_statements> }
int function_definition(){
    GET_AND_CHECK_TOKEN(token.type == TOKEN_ID, SYNTAX_ERROR);

    // check whether function with the same id is already defined and in global symtable or no
    ht_item_t *tmp = symt_search(&globalSymt, token.attribute.string->str);
    if(tmp != NULL) return SEM_DEF_FUNC_ERROR;

    if(check_id_for_keyword(token.attribute.string) != NO_ERRORS){
        // consider printing an exact error message
        // "error: trying to define a function using reserved keyword"
        
        return SYNTAX_ERROR;
    }

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

    symt_init(&localSymt);

    GET_AND_CHECK_TOKEN(token.type == TOKEN_LEFT_PAR, SYNTAX_ERROR);
    code = list_of_parameters(item);        
    CHECK_ERROR(code);
    // here we've already collected all the parameters of defined function

    GET_AND_CHECK_TOKEN(token.type == TOKEN_COLON, SYNTAX_ERROR);

    code = list_of_datatypes_ret(item);
    CHECK_ERROR(code);

    GET_AND_CHECK_TOKEN(token.type == TOKEN_LEFT_BR, SYNTAX_ERROR);
    inFunctionDefinition = true;

    // collect statements inside of a func into local symt and generate instructions for them
    
    code = list_of_statements();
    CHECK_ERROR(code);
    

    // when we're done with function definition, 
    // meaning that we've collected every statement inside of it
    // and we've generated all the instructions in this function (not sure about that tho)
    // we should clear the local symtable
    item->data.func->defined = true;
    symt_free(&localSymt);
    return NO_ERRORS;
}

/*
24. <list_of_parameters> -> ε
25. <list_of_parameters> -> <parameter> <list_of_parameters_n>
*/
int list_of_parameters(ht_item_t* item){
    GET_TOKEN();
    if(token.type == TOKEN_RIGHT_PAR){
        str_add_char(&item->data.func->argv, (char)0);
    } else {        
        code = parameter(item);
        CHECK_ERROR(code);

        code = list_of_call_parameters_n(item);   
        CHECK_ERROR(code);
    }
}

//26. <parameter> -> <list_of_datatypes> <variable>
int parameter(ht_item_t* item){
    // here we already have the datatype token
    symt_datatype_t varDatatype;
    // printf_token_debug(token);
    CHECK_TOKEN(token.type == TOKEN_KEY_W, SYNTAX_ERROR);

    switch (token.attribute.keyword){
        case K_INT:
            varDatatype = INTEGER_DT;            
            if(str_add_char(&item->data.func->argv, 'I') == 1) return INTERNAL_ERROR;
            break;

        case K_FLOAT:
            varDatatype = FLOAT_DT;
            if(str_add_char(&item->data.func->argv, 'F') == 1) return INTERNAL_ERROR;
            break;

        case K_STRING:
            varDatatype = STRING_DT;
            if(str_add_char(&item->data.func->argv, 'S') == 1) return INTERNAL_ERROR;
            break;

        case K_INT_N:
            varDatatype = INTEGER_N_DT;
            if(str_add_char(&item->data.func->argv, '?') == 1) return INTERNAL_ERROR;
            if(str_add_char(&item->data.func->argv, 'I') == 1) return INTERNAL_ERROR;
            break;

        case K_FLOAT_N:
            varDatatype = FLOAT_N_DT;
            if(str_add_char(&item->data.func->argv, '?') == 1) return INTERNAL_ERROR;
            if(str_add_char(&item->data.func->argv, 'F') == 1) return INTERNAL_ERROR;
            break;
        
        case K_STRING_N:
            varDatatype = STRING_N_DT;
            if(str_add_char(&item->data.func->argv, '?') == 1) return INTERNAL_ERROR;
            if(str_add_char(&item->data.func->argv, 'S') == 1) return INTERNAL_ERROR;
            break;
        
        default:
            break;
    }
    

    GET_AND_CHECK_TOKEN(token.type == TOKEN_ID, SYNTAX_ERROR);
    
    code = variable();
    CHECK_ERROR(code);

    symt_add_symb(&localSymt, token.attribute.string);

    ht_item_t* localItem = symt_search(&localSymt, token.attribute.string->str);
    if(localItem == NULL) return INTERNAL_ERROR; // something went wrong with htable
    
    localItem->type = var;
    localItem->data.var = malloc(sizeof(symt_var_t));
    if(localItem->data.var == NULL) return ALLOCATION_ERROR;

    localItem->data.var->type = varDatatype;
    localItem->data.var->attr = &token.attribute;


    return NO_ERRORS;
}

/*
33. <list_of_parameters_n> -> ε
34. <list_of_parameters_n> -> , <parameter> <list_of_parameters_n>
*/
int list_of_parameters_n(ht_item_t* item){
    GET_TOKEN();
    if(token.type == TOKEN_RIGHT_PAR){
        return NO_ERRORS;
    } else if (token.type == TOKEN_COMMA){
        GET_TOKEN();
        code = parameter(item);
        CHECK_ERROR(code);

        code = list_of_parameters_n(item);
        CHECK_ERROR(code);
    }
}


/*
35. <list_of_datatypes_ret> -> void
36. <list_of_datatypes_ret> -> <list_of_datatypes>
*/
int list_of_datatypes_ret(ht_item_t* item){
    // here we have a colon token so we should get the next one 
    GET_AND_CHECK_TOKEN(token.type == TOKEN_KEY_W, SYNTAX_ERROR);

    switch (token.attribute.keyword){
        case K_INT:
            if(str_add_char(&item->data.func->ret, 'I') == 1) return INTERNAL_ERROR;
            break;

        case K_FLOAT:
            if(str_add_char(&item->data.func->ret, 'F') == 1) return INTERNAL_ERROR;
            break;

        case K_STRING:
            if(str_add_char(&item->data.func->ret, 'S') == 1) return INTERNAL_ERROR;
            break;

        case K_INT_N:
            if(str_add_char(&item->data.func->ret, '?') == 1) return INTERNAL_ERROR;
            if(str_add_char(&item->data.func->ret, 'I') == 1) return INTERNAL_ERROR;
            break;

        case K_FLOAT_N:
            if(str_add_char(&item->data.func->ret, '?') == 1) return INTERNAL_ERROR;
            if(str_add_char(&item->data.func->ret, 'F') == 1) return INTERNAL_ERROR;
            break;
        
        case K_STRING_N:
            if(str_add_char(&item->data.func->ret, '?') == 1) return INTERNAL_ERROR;
            if(str_add_char(&item->data.func->ret, 'S') == 1) return INTERNAL_ERROR;
            break;

        case K_VOID:
            if(str_add_char(&item->data.func->ret, 'V') == 1) return INTERNAL_ERROR;
            break;
        
        default:
            return SYNTAX_ERROR;
            break;
    }

    return NO_ERRORS;
}


// 14. <variable_definition> -> <variable> = <var_def_expr>
int variable_definition(){
    /* 
        check if we are inside of a function or no
        it matters cuz we need to know whether we should use 
        local symtable or global symtable

        (!) function <var_def_expr> itself checking if we are inside of a function or no
    */

    code = variable();
    CHECK_ERROR(code);

    // here we still have token ID, which is <variable> in the rule above

    GET_AND_CHECK_TOKEN(token.type == TOKEN_ASSIGN, SYNTAX_ERROR);

    code = var_def_expr();
    CHECK_ERROR(code);
}

// 15. <var_def_expr> -> <function_call> <-- extension (!)
// 16. <var_def_expr> -> <expression>;
int var_def_expr(){
    GET_TOKEN();
    string_t expression;
    str_init(&expression);

    // after this GET_TOKEN() we either have an ID token (of a function)
    // or the start of expression

    if(token.type == TOKEN_ID){
        if(inFunctionDefinition){
            ht_item_t *item = symt_search(&localSymt, token.attribute.string->str);
            // ! theoretically ! we can't have any 'func' type of items in local symtable
            // cuz there are only variables in it
            if(item->type == var){
                // collecting an expression till semicolon token is found
                do{
                    GET_TOKEN();
                } while(token.type != TOKEN_SEMICOLON);
                return NO_ERRORS;
            }
        }
    } else {
        // collecting an expression till semicolon token is found
        do{
            GET_TOKEN();
        } while(token.type != TOKEN_SEMICOLON);
        
        return NO_ERRORS;
    }
}

static string_t collecting_an_expression(){
    // getting tokens and copying their attributes into a string
    string_t expression;
    str_init(&expression);

    while(token.type != TOKEN_SEMICOLON){
        switch (token.type){
        case TOKEN_ID:
            str_copy_string(&expression, token.attribute.string);
            break;
        
        case TOKEN_PLUS:
            str_add_char(&expression, '+');
            break;

        case TOKEN_MINUS:
            str_add_char(&expression, '-');
            break;
        
        case TOKEN_MUL:
            str_add_char(&expression, '*');
            break;
        
        case TOKEN_DIV:
            str_add_char(&expression, '/');
            break;

        case TOKEN_CONC:
            str_add_char(&expression, '.');
            break;

        /* TODO: 
            1. add other symbols that we can use in expressions
            2. decide whether we should distinguish between an assignment expression
            and logical expression (such as inside of if() and while()) 
            3. i don't even know if it's a good idea or no to write an expression that way
        */  
        }
        str_add_char(&expression, ' ');
        GET_TOKEN();
    }

    //"$a + $b * 10"

    return expression;
}

//17. <function_call> -> ID( <list_of_call_parameters> );
int function_call(){
    ht_item_t* item = symt_search(&globalSymt, token.attribute.string->str);
    printf("\tfound a function call with id '%s'\n", item->key);
    if(item == NULL) {
        return SEM_DEF_FUNC_ERROR;
    } else if(item->type == func){
        GET_AND_CHECK_TOKEN(token.type == TOKEN_LEFT_PAR, SYNTAX_ERROR);
        
        code = list_of_call_parameters(item);
        CHECK_ERROR(code);

        // here we should generate an instruction with function call
    } else return INTERNAL_ERROR;
    return NO_ERRORS;
}

//20. <call_parameter> -> <variable>
//21. <call_parameter> -> "string" // ?????
int call_parameter(ht_item_t* function){
    switch (token.type){
    case TOKEN_TYPE_STRING:
        printf("[string_call_param] %s\n", token.attribute.string->str);
        break;
    
    case TOKEN_TYPE_INT:
        printf("[int_call_param] %d\n", token.attribute.integer);
        break;

    case TOKEN_TYPE_FLOAT:
        printf("[float_call_param] %f\n", token.attribute.decimal);
        break;

    case TOKEN_ID:
        code = variable();        
        CHECK_ERROR(code);
        break;
    default:
        return SYNTAX_ERROR;
        break;
    }
    
    // string check

    return NO_ERRORS;
}

//18. <list_of_call_parameters> -> ε
//19. <list_of_call_parameters> -> <call_parameter> <list_of_call_parameters_n>
int list_of_call_parameters(ht_item_t* function){
    GET_TOKEN();
    if(token.type == TOKEN_RIGHT_PAR){
        if(!str_cmp_const_str(&function->data.func->argv, "")){
            GET_AND_CHECK_TOKEN(token.type == TOKEN_SEMICOLON, SYNTAX_ERROR);
            // generate a function call for function with no parameters
            return NO_ERRORS;
        } else return SEM_TYPE_ERROR;
    } else if (token.type == TOKEN_ID || token.type == TOKEN_TYPE_STRING || 
               token.type == TOKEN_TYPE_INT || token.type == TOKEN_TYPE_FLOAT){
        code = call_parameter(function);
        CHECK_ERROR(code);
        
        code = list_of_call_parameters_n(function);
        CHECK_ERROR(code);
        // generate a function call after we've collected all the call parameters
    } else return SYNTAX_ERROR;

    return NO_ERRORS;
}

/*
22. <list_of_call_parameters_n> -> , <call_parameter> <list_of_call_parameters_n>
23. <list_of_call_parameters_n> -> ε 
*/
int list_of_call_parameters_n(ht_item_t* function){
    GET_TOKEN();
    if(token.type == TOKEN_RIGHT_PAR){
        return NO_ERRORS;
    } else if (token.type == TOKEN_COMMA){
        GET_TOKEN();
        code = call_parameter(function);
        CHECK_ERROR(code);
        

        code = list_of_call_parameters_n(function);
        CHECK_ERROR(code);
        // generate a function call
    }
}


int check_id_for_keyword(string_t* word){
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

    int i = 0;
    while(i < KEYW_COUNT){
        if(str_cmp_string(word, &keywList[i++]) == 0)
            return SEM_OTHER_ERROR;
    }

    return NO_ERRORS;
}

/* 
    32. <variable> -> $ID
    function that checks whether current ID is correct
    meaning it doesn't contain any forbidden words (reserved, keywords, etc.)
*/
int variable(){
    /*  the only case we are going to get into this function
        is where we currently have token with token.type == TOKEN_ID
        or a token that goes strictly (theoretically) before TOKEN_ID

        therefore we can safely use check at the line below  
    
        FIXME: in the future i would like to fix this to avoid even theoreticall situations where this could be wrong
               so the function variable() would be called ONLY when we already have an ID token
               or ONLY the one before an ID */
    
    
    
    if(token.type != TOKEN_ID) GET_AND_CHECK_TOKEN(token.type == TOKEN_ID, SYNTAX_ERROR);

    string_t varId;
    if(str_init(&varId) == 1) return ALLOCATION_ERROR;
    
    if(token.attribute.string->str[0] == '$'){
        int i = 1;
        while(token.attribute.string->str[i] != '\0'){
            str_add_char(&varId, token.attribute.string->str[i++]);
        }

        if(check_id_for_keyword(&varId) != NO_ERRORS) {
            /*
                consider printing an exact error message
                "error: trying to define a variable using reserved keyword"
            */
            str_free(&varId);
            return SEM_OTHER_ERROR;
        }
    } else {
        str_free(&varId);
        return SYNTAX_ERROR;
    }

    str_free(&varId);
    return NO_ERRORS;
}