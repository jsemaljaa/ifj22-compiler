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

token_t token;
int ret;

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

#define CHECK_TOKEN(expr)               \
    if(!(expr)) return false;           \

#define GET_AND_CHECK_TOKEN(expr)       \
    GET_TOKEN();                        \
    CHECK_TOKEN((expr));                \


static void printf_token_debug(token_t token){
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
}

int parse(parser_data_t* pData) {
    get_next_token(&token);     
    if(token.type == TOKEN_END_OF_FILE) return true;


    if(!program(pData)) exit_error(SYNTAX_ERROR); 
}


bool program(parser_data_t* pData){
    switch (token.type){
    case TOKEN_PROLOG:
        if(!prolog(pData)) return false;
        
        if(!list_of_statements(pData)) return false;
    default:
        return false;
        break;
    }    
}

bool prolog(parser_data_t* pData){

    GET_AND_CHECK_TOKEN(token.type == TOKEN_ID);
    if(str_cmp_const_str(token.attribute.string, "declare")) return false;
    
    GET_AND_CHECK_TOKEN(token.type == TOKEN_LEFT_PAR);

    GET_AND_CHECK_TOKEN(token.type == TOKEN_ID);
    if(str_cmp_const_str(token.attribute.string, "strict_types")) return false;

    GET_AND_CHECK_TOKEN(token.type == TOKEN_ASSIGN);
        
    GET_AND_CHECK_TOKEN(token.type == TOKEN_TYPE_INT);
    if(token.attribute.integer != 1) return false; 

    
    GET_AND_CHECK_TOKEN(token.type == TOKEN_RIGHT_PAR);
    GET_AND_CHECK_TOKEN(token.type == TOKEN_SEMICOLON);

    return true;
}

//3. <list_of_statements> -> EOF
//4. <list_of_statements> -> <statement> <list_of_statements>
bool list_of_statements(parser_data_t* pData){
    GET_TOKEN();
    if(token.type == TOKEN_END_OF_FILE) return true;
    else {
        switch (token.type){
            case TOKEN_ID:
                if(!statement(pData)) return false;
                return true;
            case TOKEN_KEY_W:
                if(token.attribute.keyword == K_IF || token.attribute.keyword == K_RETURN || token.attribute.keyword == K_WHILE){
                if(!statement(pData)) return false;
                }
            return false;
        }
    }
}

//5. <statement> -> <function_definition>
//6. <statement> -> <variable_definition>
//7. <statement> -> <function_call>
//8. <statement> -> if ( <expression> ) { <list_of_statements> } else { <list_of_statements> }
//9. <statement> -> while ( <expression> ) { <list_of_statements> }
//10. <statement> -> return <return_expressions>;
bool statement(parser_data_t* pData){
    bool internal = false;
    for(int i = 0; i < 10; i++){
        if(!str_cmp_const_str(token.attribute.string, internals[i])) internal = true; 
    }

    CHECK_TOKEN(token.type == TOKEN_KEY_W);
    if(!str_cmp_const_str(token.attribute.string, "function")){
        GET_AND_CHECK_TOKEN(token.type == TOKEN_ID);
        symt_add_symb(&(pData->globalSymt), token.attribute.string);
    }
}