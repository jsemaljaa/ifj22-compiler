/*
 * IFJ2022/project/error.h 
 * 
 * @brief Error handler interface declaration
 * 
 * @author Mazurava Maryia <xmazur08@vutbr.cz>
 */


#ifndef _ERROR_H
#define _ERROR_H

#define NO_ERRORS 0 
#define LEXICAL_ERROR 1  
#define SYNTAX_ERROR 2
#define SEM_DEF_FUNC_ERROR 3 
#define SEM_TYPE_ERROR 4
#define SEM_VAR_ERROR 5 
#define SEM_STMT_FUNC_ERROR 6 
#define SEM_TYPE_COMP_ERROR 7 
#define SEM_OTHER_ERROR 8
#define INTERNAL_ERROR 99
#define ALLOCATION_ERROR 9

int exit_error(int error_name);

#endif // _ERROR_H