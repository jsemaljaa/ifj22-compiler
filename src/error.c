/*
 * IFJ2022/project/error.c 
 * 
 * @brief Error handler implementation
 * 
 * @author Mazurava Maryia <xmazur08@vutbr.cz>
 */


#include <stdio.h>
#include <stdlib.h>
#include "error.h"
#include "parser.h"
#include "symtable.h"

int exit_error(int error_name) {
    int error_number = 0;
    switch (error_name) {
        case LEXICAL_ERROR:
            fprintf(stderr, "1 - Error with lexical analysis.\n");
            error_number = 1;
            break;
        case SYNTAX_ERROR:
            fprintf(stderr, "2 - Error with syntactic analysis.\n");
            error_number = 2;
            break;
        case SEM_DEF_FUNC_ERROR:
            fprintf(stderr, "3 - Error with semantic analysis: function is not defined or attempt to redefine a function.\n");
            error_number = 3;
            break;
        case SEM_TYPE_ERROR:
            fprintf(stderr, "4 - Error with semantic analysis: wrong number/type of parameters in a function call or type of return value.\n");
            error_number = 4;
            break;
        case SEM_FUNC_ERROR:
            fprintf(stderr, "5 - Error with semantic analysis: using of undefined variable.\n");
            error_number = 5;
            break;
        case SEM_STMT_FUNC_ERROR:
            fprintf(stderr, "6 - Error with semantic analysis: missing/extra expression in function return command.\n");
            error_number = 6;
            break;
        case SEM_TYPE_COMP_ERROR:
            fprintf(stderr, "7 - Error with semantic analysis: wrong type compatibility in expressions.\n");
            error_number = 7;
            break;
        case SEM_OTHER_ERROR:
            fprintf(stderr, "8 -  Other semantic errors.\n");
            error_number = 8;
            break;
        case INTERNAL_ERROR:
            fprintf(stderr, "99 - Internal error.\n");
            error_number = 99;
            break;
        case ALLOCATION_ERROR:
            fprintf(stderr, "9 - Error with allocation of the memory.\n");
            error_number = 9;
            break;
    }
  
    exit(error_number);
}