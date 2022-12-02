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
bool inWhile, inWhileExpr;
bool inIf, inIfExpr;
bool inFunctionDefinition;
string_t expr;

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


static void clean_exit(int code){
    symt_free(&globalSymt);
    symt_free(&localSymt);
    if(token.type == TOKEN_ID)
        str_free(token.attribute.string);
    exit_error(code);
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
    CHECK_RULE(symt_add_internal_functions());

    // GET_AND_CHECK_TOKEN(token.type == TOKEN_END_OF_FILE, SYNTAX_ERROR);

    CHECK_RULE(prolog());
    
    if(token.type == TOKEN_END_OF_FILE) return NO_ERRORS;

    CHECK_RULE(list_of_statements());

    printf("returning with code %d\n", code);
    clean_exit(code);  
}

int prolog(){

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
int list_of_statements(){
    while(true){
        GET_TOKEN();
        printf_token_debug(token);
        
        if(token.type == TOKEN_END_OF_FILE) {
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
int statement(){
    switch (token.type){
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
        } else if(token.attribute.keyword == K_RETURN){}
        break;
    default:
        if(inIf || inWhile) return NO_ERRORS;
        else return SYNTAX_ERROR;
    }
    
    if(token.type == TOKEN_END_OF_FILE) return NO_ERRORS;
    CHECK_RULE(list_of_statements());   
}

int inside_if(){
    // right now the token we have is IF keyword
    // according to rules we should have the '(' token next
    GET_AND_CHECK_TOKEN(token.type == TOKEN_LEFT_PAR, SYNTAX_ERROR);
    GET_TOKEN();
    str_init(&expr);
    inIfExpr = true;
    code = collecting_an_expression(&expr);
    if(code != NO_ERRORS) return code;
    /*
        check collected expression by calling a function from expressions.c interface
        if OK, then continue, otherwise return error

        last token after collecting_an_expression() should be either ')' or ';' 
        depends on a situation
    */

    GET_AND_CHECK_TOKEN(token.type == TOKEN_LEFT_BR, SYNTAX_ERROR);
    inIfExpr = false;
    inIf = true;
    
    CHECK_RULE(list_of_statements());

    GET_AND_CHECK_TOKEN(token.type == TOKEN_RIGHT_BR, SYNTAX_ERROR);
    
    
    GET_TOKEN_CHECK_KEYW(K_ELSE, SEM_STMT_FUNC_ERROR);
    GET_AND_CHECK_TOKEN(token.type == TOKEN_LEFT_BR, SYNTAX_ERROR);

    CHECK_RULE(list_of_statements());

    GET_AND_CHECK_TOKEN(token.type == TOKEN_RIGHT_BR, SYNTAX_ERROR);
    inIf = false;
    str_free(&expr);
    return NO_ERRORS;
}

int inside_while(){
    
    GET_AND_CHECK_TOKEN(token.type == TOKEN_LEFT_PAR, SYNTAX_ERROR);
    // here we should collect an expression inside of a WHILE statement, then check if it's okay and after check for the TOKEN_RIGHT_PAR
    // if(!expression_check()) return false;
    GET_TOKEN();
    str_init(&expr);
    inWhileExpr = true;
    code = collecting_an_expression(&expr);
    if(code != NO_ERRORS) return code;

    GET_AND_CHECK_TOKEN(token.type == TOKEN_LEFT_BR, SYNTAX_ERROR);
    inWhileExpr = false;
    inWhile = true;
    
    CHECK_RULE(list_of_statements());

    GET_AND_CHECK_TOKEN(token.type == TOKEN_RIGHT_BR, SYNTAX_ERROR);

    inWhile = false;
    str_free(&expr);
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

        code = list_of_parameters_n(item);   
        CHECK_ERROR(code);
    }
}

//26. <parameter> -> <list_of_datatypes> <variable>
int parameter(ht_item_t* item){
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


// 14. <variable_definition> -> <variable> = <expr>
int variable_definition(){
    /* 
        check if we are inside of a function or no
        it matters cuz we need to know whether we should use 
        local symtable or global symtable 
    */

    CHECK_RULE(variable());

    // here we still have token ID, which is <variable> in the rule above

    GET_AND_CHECK_TOKEN(token.type == TOKEN_ASSIGN, SYNTAX_ERROR);
    
    CHECK_RULE(var_def_expr());
    
    return NO_ERRORS;
}

// 15. <var_def_expr> -> <function_call> <-- FUNEXP EXTENSION !!!
// 16. <var_def_expr> -> <expression>;
int var_def_expr(){
    string_t expression;
    str_init(&expression);

    // after this GET_TOKEN() we either have an ID token (of a function)
    // or the start of expression
    
    // two cases: <function_call> and <expression>
    GET_TOKEN();
    // at this line we have either TOKEN_ID (variable or function)
    // or some different type of token which is start of an expression


    if(token.type == TOKEN_ID){ // function ID or variable ID
        if(token.attribute.string->str[0] == '$'){ // then we have variable ID
            CHECK_RULE(variable());

            // here we are checking a variable and then proceeding to collect an expression
        } 
        /*
        else { // otherwise we have function ID
            CHECK_RULE(function_call());

            return NO_ERRORS;

            // we don't need to go futher after this line
        }
        */
    }

    code = collecting_an_expression(&expression);
    if(code != NO_ERRORS) return code;
    // checking if expression is OK by calling the expressions.c interface here
    
    return NO_ERRORS;
}

static int collecting_an_expression(string_t *expression){
    // getting tokens and copying their attributes into a string
    // string_t expression;
    // str_init(&expression);
    token_type_t expected;

    if(inWhileExpr || inIfExpr){
        expected = TOKEN_RIGHT_PAR;
    } else {
        expected = TOKEN_SEMICOLON;    
    }

    while(token.type != expected){
        switch (token.type){
        case TOKEN_ID:
            str_concat(expression, token.attribute.string);
            break;
        
        case TOKEN_PLUS:
            str_add_char(expression, '+');
            break;

        case TOKEN_MINUS:
            str_add_char(expression, '-');
            break;
        
        case TOKEN_MUL:
            str_add_char(expression, '*');
            break;
        
        case TOKEN_DIV:
            str_add_char(expression, '/');
            break;

        case TOKEN_CONC:
            str_add_char(expression, '.');
            break;
        
        case TOKEN_TYPE_INT:
            str_add_char(expression, 'i');
            break;
        
        case TOKEN_TYPE_FLOAT:
            str_add_char(expression, 'f');
            break;
        
        case TOKEN_TYPE_STRING:
            str_add_char(expression, 's');
            break;

        case TOKEN_EQUAL:
            str_add_char(expression, '=');
            str_add_char(expression, '=');
            str_add_char(expression, '=');
            break;

        case TOKEN_NOT_EQUAL:
            str_add_char(expression, '!');
            str_add_char(expression, '=');
            str_add_char(expression, '=');
            break;
        
        case TOKEN_LESS:
            str_add_char(expression, '<');
            break;

        case TOKEN_GREATER:
            str_add_char(expression, '>');
            break;
        
        case TOKEN_LESS_EQ:
            str_add_char(expression, '<');
            str_add_char(expression, '=');
            break;
        
        case TOKEN_GREATER_EQ:
            str_add_char(expression, '>');
            str_add_char(expression, '=');
            break;
        
        case TOKEN_LEFT_PAR:
            str_add_char(expression, '(');
            break;
        
        case TOKEN_RIGHT_PAR:
            if(!inWhileExpr && !inIfExpr){
                str_add_char(expression, ')');
            } else {
                str_free(expression);
                clean_exit(SYNTAX_ERROR);
            }
            break;
        
        case TOKEN_KEY_W:
            if(token.attribute.keyword == K_NULL){
                str_add_char(expression, 'n');
            } else {
                str_free(expression);
                clean_exit(SYNTAX_ERROR);
            }
            break;

        default:
            str_free(expression);
            clean_exit(SYNTAX_ERROR);
            break;
        }

        GET_TOKEN();
        str_add_char(expression, ' ');
    }

    return NO_ERRORS;
}

//17. <function_call> -> ID( <list_of_call_parameters> );   
int function_call(){
    ht_item_t* item = symt_search(&globalSymt, token.attribute.string->str);
    //  printf("\tfound a function call with id '%s'\n", item->key);

    if(item == NULL) {
        return SEM_DEF_FUNC_ERROR;
    } else if(item->type == func){        
        GET_AND_CHECK_TOKEN(token.type == TOKEN_LEFT_PAR, SYNTAX_ERROR);
        
        code = list_of_call_parameters(item);
        CHECK_ERROR(code);

        // we can return here with tokens (no errors):
        //          1) TOKEN_RIGHT_PAR
        //          2) TOKEN_SEMICOLON
        //          

    } else return INTERNAL_ERROR;
    return NO_ERRORS;
}

//20. <call_parameter> -> <variable>
//21. <call_parameter> -> "string" // ?????
int call_parameter(ht_item_t* function, string_t params){
    switch (token.type){
    case TOKEN_TYPE_STRING:
        // generator_internal_func(function->key, "string", token.attribute.string->str);
        break;
    
    case TOKEN_TYPE_INT:
        // generator_internal_func(function->key, "int", token.attribute.integer);
        break;

    case TOKEN_TYPE_FLOAT:
        // generator_internal_func(function->key, "float", token.attribute.decimal);
        break;

    case TOKEN_ID:
        CHECK_RULE(variable());

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
    string_t params;
    str_init(&params);

    GET_TOKEN();
    if(token.type == TOKEN_RIGHT_PAR){
        if(!str_cmp_const_str(&function->data.func->argv, "")){
            GET_AND_CHECK_TOKEN(token.type == TOKEN_SEMICOLON, SYNTAX_ERROR);

            // generate a function call for function with no parameters
            
            return NO_ERRORS;
        } else return SEM_TYPE_ERROR;
    } else if (token.type == TOKEN_ID || token.type == TOKEN_TYPE_STRING || 
               token.type == TOKEN_TYPE_INT || token.type == TOKEN_TYPE_FLOAT){
        code = call_parameter(function, params);
        CHECK_ERROR(code);

        code = list_of_call_parameters_n(function, params);
        CHECK_ERROR(code);

    } else return SYNTAX_ERROR;

    return NO_ERRORS;
}

/*
22. <list_of_call_parameters_n> -> , <call_parameter> <list_of_call_parameters_n>
23. <list_of_call_parameters_n> -> ε 
*/
int list_of_call_parameters_n(ht_item_t* function, string_t params){
    GET_TOKEN();
    if(token.type == TOKEN_RIGHT_PAR){

        // generate a function call
        
        return NO_ERRORS;
    } else if (token.type == TOKEN_COMMA){
        GET_TOKEN();
        code = call_parameter(function, params);
        CHECK_ERROR(code);
    
        code = list_of_call_parameters_n(function, params);
        CHECK_ERROR(code);
    }
}


int check_id_for_keyword(string_t *word){
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

    ht_item_t* var;

    if(inFunctionDefinition){
        var = symt_search(&localSymt, token.attribute.string->str);
        if(var == NULL){
            symt_add_symb(&localSymt, token.attribute.string);
        }
    } else {
        var = symt_search(&globalSymt, token.attribute.string->str);
        if(var == NULL){
            symt_add_symb(&globalSymt, token.attribute.string);
        }
    }

    string_t varId;
    if(str_init(&varId) != 0) return ALLOCATION_ERROR;
    
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