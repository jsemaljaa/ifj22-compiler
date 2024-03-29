/*
 * IFJ2022/project/parser.c 
 * 
 * @brief Syntactic and semantic analysis implementation
 * 
 * @author Vinogradova Alina <xvinog00@vutbr.cz>
 * @author Mazurava Maryia <xmazur08@vutbr.cz>
 */


#include "parser.h"

htable globalSymt;
htable localSymt;
token_t token;
int ret;
int code;
bool inWhile, whileExpr;
bool inIf, ifExpr;
bool inFunctionDefinition, inFunctionCall;
bool local;
string_t expr;
int symbolsCnt;

static void printf_symtable_func_debug(ht_item_t* item){
    printf("--- HT_ITEM FUNCTION DATA ---\n");
    printf("Function key is: %s\n", item->key);
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

void printf_token_debug(token_t token){
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


static int clean_exit(int code){
    symt_free(&globalSymt);
    symt_free(&localSymt);
    if(token.type == TOKEN_ID)
        str_free(token.attribute.string);
    // exit_error(code);
    return code;
}


static int symt_add_internal_functions(){
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
    CHECK_RULE(symt_add_internal_functions());

    // GET_AND_CHECK_TOKEN(token.type == TOKEN_END_OF_FILE, SYNTAX_ERROR);
    
    CHECK_RULE(prolog());
    
    generator_header();
    if(token.type == TOKEN_END_OF_FILE) {
        // generator_end_of_file();
        return NO_ERRORS;
    }
    
    CHECK_RULE(list_of_statements());

    // generator_end_of_file();
    clean_exit(code);  
}

static int prolog(){

    GET_TOKEN();
    
    if(token.type == TOKEN_END_OF_FILE || token.type != TOKEN_PROLOG) return SYNTAX_ERROR;
    
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
static int list_of_statements(){
    while(true){
        GET_TOKEN();
        if(token.type == TOKEN_END_OF_FILE){
            return NO_ERRORS;
        }
        CHECK_RULE(statement());
        return NO_ERRORS;
    }
}

//5. <statement> -> <function_definition>
//6. <statement> -> <variable_definition>
//7. <statement> -> <function_call>
//8. <statement> -> if ( <expression> ) { <list_of_statements> } else { <list_of_statements> }
//9. <statement> -> while ( <expression> ) { <list_of_statements> }
//10. <statement> -> return <return_expressions>;
static int statement(){
    local = inIf || inWhile || inFunctionDefinition;
    switch (token.type){
    case TOKEN_END_OF_FILE:
        return NO_ERRORS;
    case TOKEN_ID:
        if(token.attribute.string->str[0] == '$'){
            CHECK_RULE(variable_definition());
        } else {
            CHECK_RULE(function_call());   // internal function OR user function call
        }
        break;
    case TOKEN_KEY_W:
        if(token.attribute.keyword == K_FUNCTION){
            // we can't have function definition inside of an other function definition
            CHECK_RULE(function_definition());
        } else if(token.attribute.keyword == K_IF){
            CHECK_RULE(inside_if());
        } else if(token.attribute.keyword == K_WHILE){
            CHECK_RULE(inside_while());
        } else if(token.attribute.keyword == K_RETURN){
            CHECK_RULE(return_statement());
        }
        break;
    case TOKEN_TYPE_INT:
    case TOKEN_TYPE_FLOAT:
    case TOKEN_TYPE_STRING:
        if(local){
            code = parse_expression(&localSymt, 0);
        } else code = parse_expression(&globalSymt, 0);
            
        CHECK_ERROR(code);
        break;
    default:
        if(local) return NO_ERRORS;
        else {
            // printf_token_debug(token);
            return SYNTAX_ERROR;
        }
    }
    
    if(token.type == TOKEN_END_OF_FILE) return NO_ERRORS;
    CHECK_RULE(list_of_statements());   
}

static int return_statement(){
    GET_TOKEN();

    if(token.type == TOKEN_SEMICOLON) return NO_ERRORS;

    if(local) code = parse_expression(&localSymt, 0);
    else code = parse_expression(&globalSymt, 0);

    CHECK_ERROR(code);

    return NO_ERRORS;
}

static int inside_if(){
    // right now the token we have is IF keyword
    // according to rules we should have the '(' token next
    GET_AND_CHECK_TOKEN(token.type == TOKEN_LEFT_PAR, SYNTAX_ERROR);
    
    // CHECK_RULE(parse_expression());
    do {
        GET_TOKEN();
    } while(token.type != TOKEN_RIGHT_PAR);


    /*
        check collected expression by calling a function from expressions.c interface
        if OK, then continue, otherwise return error

        last token after collecting_an_expression() should be either ')' or ';' 
        depends on a situation
    */

    GET_AND_CHECK_TOKEN(token.type == TOKEN_LEFT_BR, SYNTAX_ERROR);
        
    inIf = true;
    generator_start_if();

    CHECK_RULE(list_of_statements());
    CHECK_TOKEN(token.type == TOKEN_RIGHT_BR, SYNTAX_ERROR);

    generator_end_if();

    GET_TOKEN_CHECK_KEYW(K_ELSE, SEM_STMT_FUNC_ERROR);
    GET_AND_CHECK_TOKEN(token.type == TOKEN_LEFT_BR, SYNTAX_ERROR);

    generator_start_else();

    CHECK_RULE(list_of_statements());
    CHECK_TOKEN(token.type == TOKEN_RIGHT_BR, SYNTAX_ERROR);
    
    generator_end_else();
    inIf = false;

    return NO_ERRORS;
}

static int inside_while(){
    GET_AND_CHECK_TOKEN(token.type == TOKEN_LEFT_PAR, SYNTAX_ERROR);
    // here we should collect an expression inside of a WHILE statement, then check if it's okay and after check for the TOKEN_RIGHT_PAR
    // if(!expression_check()) return false;

    // code = parse_expression(&localSymt, 1);


    // CHECK_RULE(parse_expression());
    do {
        GET_TOKEN();
    } while(token.type != TOKEN_RIGHT_PAR);

    GET_AND_CHECK_TOKEN(token.type == TOKEN_LEFT_BR, SYNTAX_ERROR);
    
    inWhile = true;
    generator_start_while();

    CHECK_RULE(list_of_statements());

    CHECK_TOKEN(token.type == TOKEN_RIGHT_BR, SYNTAX_ERROR);

    inWhile = false;
    generator_end_while();

    return NO_ERRORS;
}


// 13. <function_definition> -> function ID( <list_of_parameters> ) : <list_of_datatypes_ret> { <list_of_statements> }
static int function_definition(){
    GET_AND_CHECK_TOKEN(token.type == TOKEN_ID, SYNTAX_ERROR);

    if(check_id_for_keyword(token.attribute.string) != NO_ERRORS){
        // consider printing an exact error message
        // "error: trying to define a function using reserved keyword"    
        return SYNTAX_ERROR;
    }

    // check whether function with the same id is already defined and in global symtable or no
    ht_item_t *tmp = symt_search(&globalSymt, token.attribute.string->str);
    if(tmp != NULL) return SEM_DEF_FUNC_ERROR;

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
    generator_start_func(item);
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

    CHECK_TOKEN(token.type == TOKEN_RIGHT_BR, SYNTAX_ERROR);

    // when we're done with function definition, 
    // meaning that we've collected every statement inside of it
    // and we've generated all the instructions in this function (not sure about that tho)
    // we should clear the local symtable

    generator_end_func(item);
    symt_free(&localSymt);
    return NO_ERRORS;
}

/*
24. <list_of_parameters> -> ε
25. <list_of_parameters> -> <parameter> <list_of_parameters_n>
*/
static int list_of_parameters(ht_item_t* item){
    GET_TOKEN();
    if(token.type == TOKEN_RIGHT_PAR){
        str_add_char(&item->data.func->argv, (char)0);
    } else {        
        code = parameter(item);
        CHECK_ERROR(code);

        code = list_of_parameters_n(item);   
        CHECK_ERROR(code);
    }
}

//26. <parameter> -> <list_of_datatypes> <variable>
static int parameter(ht_item_t* item){
    // here we already have the datatype token
    symt_datatype_t varDatatype;
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
    
    if(str_add_char(&item->data.func->argv, ' ') == 1) return INTERNAL_ERROR;

    GET_AND_CHECK_TOKEN(token.type == TOKEN_ID, SYNTAX_ERROR);
    
    CHECK_RULE(variable());

    // symt_add_symb(&localSymt, token.attribute.string);

    ht_item_t* localItem = symt_search(&localSymt, token.attribute.string->str);
    if(localItem == NULL) return SEM_OTHER_ERROR; // something went wrong with htable
    
    localItem->data.var = malloc(sizeof(symt_var_t));
    if(localItem->data.var == NULL) return ALLOCATION_ERROR;

    localItem->type = var;

    localItem->data.var->type = varDatatype;
    localItem->data.var->attr = &token.attribute;

    // generate function parameter
    if(generator_check_var(localItem)){
        generator_get_new_var(localItem);
    }


    return NO_ERRORS;
}

/*
33. <list_of_parameters_n> -> ε
34. <list_of_parameters_n> -> , <parameter> <list_of_parameters_n>
*/
static int list_of_parameters_n(ht_item_t* item){
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
static int list_of_datatypes_ret(ht_item_t* item){
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
static int variable_definition(){

    CHECK_RULE(variable());

    // ht_item_t *var;
    // // here we still have token ID, which is <variable> in the rule above
    // if(local) var = symt_search(&localSymt, token.attribute.string->str);
    // else var = symt_search(&globalSymt, token.attribute.string->str); 

    // var->data.var = malloc(sizeof(symt_var_t));
    // if(var->data.var == NULL) return ALLOCATION_ERROR;
    // // var->type = var;
    // var->data.var->type = UNDEFINED_DT;
    
    // var->data.var->attr = malloc(sizeof(token_attribute_t));

    // str_init(var->data.var->attr->string);
    // printf("qwrqwwqrasasfasfqwfa\n");
    // var->data.var->attr->integer = 0;
    // var->data.var->attr->decimal = 0.0;
    

    CHECK_RULE(var_def_expr());
    return NO_ERRORS;
}

// 15. <var_def_expr> -> <function_call> <-- FUNEXP EXTENSION !!!
// 16. <var_def_expr> -> <expression>;
static int var_def_expr(){
    // TOKEN_ID here
    if(inIf || inWhile || inFunctionDefinition){
        code = parse_expression(&localSymt, 0);
    } else code = parse_expression(&globalSymt, 0);
    
    if(code == -1){
        CHECK_RULE(function_call());
        return NO_ERRORS;
    }
    
    
    CHECK_ERROR(code);
}

//17. <function_call> -> ID( <list_of_call_parameters> );   
static int function_call(){
    ht_item_t* item = symt_search(&globalSymt, token.attribute.string->str);
    // printf("\tfound a function call with id '%s'\n", item->key);
    if(item == NULL){
        return SEM_DEF_FUNC_ERROR;
    } else if(item->type == func){        
        GET_AND_CHECK_TOKEN(token.type == TOKEN_LEFT_PAR, SYNTAX_ERROR);
        inFunctionCall = true;

        code = list_of_call_parameters(item);
        CHECK_ERROR(code);
        
        GET_AND_CHECK_TOKEN(token.type == TOKEN_SEMICOLON, SYNTAX_ERROR);
        inFunctionCall = false;
        generator_internal_func(item->key);
        return NO_ERRORS;
    } else {
        return INTERNAL_ERROR;
    }
}

//18. <list_of_call_parameters> -> ε
//19. <list_of_call_parameters> -> <call_parameter> <list_of_call_parameters_n>
static int list_of_call_parameters(ht_item_t* function){
    string_t params;
    str_init(&params);

    // token == TOKEN_LEFT_PAR
    GET_TOKEN();

    if(token.type == TOKEN_RIGHT_PAR){
        if(!strcmp(function->key, "write")){
            return NO_ERRORS;
        }
        else if(!str_cmp_const_str(&function->data.func->argv, "")){
            // GET_AND_CHECK_TOKEN(token.type == TOKEN_SEMICOLON, SYNTAX_ERROR);
            // generator_call_func(function);
            return NO_ERRORS;
        }
        else return SEM_TYPE_ERROR;
    } else if (token.type == TOKEN_ID || token.type == TOKEN_TYPE_STRING || 
               token.type == TOKEN_TYPE_INT || token.type == TOKEN_TYPE_FLOAT){
        code = call_parameter(function, params);
        CHECK_ERROR(code);

        code = list_of_call_parameters_n(function, params);
        CHECK_ERROR(code);

        return NO_ERRORS;

    } else return SYNTAX_ERROR;

    return NO_ERRORS;
}

//20. <call_parameter> -> <variable>
//21. <call_parameter> -> "string" // ?????
static int call_parameter(ht_item_t* function, string_t params){
    switch (token.type){
    case TOKEN_TYPE_STRING:
        generator_get_arg(function->key, token, UNDEFINED_DT);
        break;
    
    case TOKEN_TYPE_INT:
        generator_get_arg(function->key, token, UNDEFINED_DT);
        break;

    case TOKEN_TYPE_FLOAT:
        generator_get_arg(function->key, token, UNDEFINED_DT);
        break;

    case TOKEN_ID:
        CHECK_RULE(variable());
        ht_item_t *var; 
        if(local) var = symt_search(&localSymt, token.attribute.string->str);
        else var = symt_search(&globalSymt, token.attribute.string->str);
        generator_get_arg(function->key, token, var->data.var->type);
        break;
    default:
        return SYNTAX_ERROR;
        break;
    }

    return NO_ERRORS;
}

/*
22. <list_of_call_parameters_n> -> , <call_parameter> <list_of_call_parameters_n>
23. <list_of_call_parameters_n> -> ε 
*/
static int list_of_call_parameters_n(ht_item_t* function, string_t params){
    GET_TOKEN();
    
    if(token.type == TOKEN_RIGHT_PAR){
        generator_call_func(function);
        return NO_ERRORS;
    } else if (token.type == TOKEN_COMMA){
        GET_TOKEN();
        code = call_parameter(function, params);
        CHECK_ERROR(code);
    
        code = list_of_call_parameters_n(function, params);
        CHECK_ERROR(code);
    }
}


static int check_id_for_keyword(string_t *word){
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
static int variable(){
    /*  the only case we are going to get into this function
        is where we currently have token with token.type == TOKEN_ID
        or a token that goes strictly (theoretically) before TOKEN_ID

        therefore we can safely use check at the line below  
    
        FIXME: in the future i would like to fix this to avoid even theoreticall situations where this could be wrong
               so the function variable() would be called ONLY when we already have an ID token
               or ONLY the one before an ID */

    if(token.type != TOKEN_ID) GET_AND_CHECK_TOKEN(token.type == TOKEN_ID, SYNTAX_ERROR);

    ht_item_t* var;

    if(local){
        var = symt_search(&localSymt, token.attribute.string->str);
        if(var == NULL) symt_add_symb(&localSymt, token.attribute.string);
        var = symt_search(&localSymt, token.attribute.string->str);
    } else {
        var = symt_search(&globalSymt, token.attribute.string->str);
        if(var == NULL) symt_add_symb(&globalSymt, token.attribute.string);
        var = symt_search(&globalSymt, token.attribute.string->str);
    }

    var->data.var = malloc(sizeof(symt_var_t *));
    if(var->data.var == NULL) return ALLOCATION_ERROR;
    var->data.var->type = UNDEFINED_DT;

    return (token.attribute.string->str[0] == '$') ? NO_ERRORS : SYNTAX_ERROR; 
}