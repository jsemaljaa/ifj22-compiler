/*
 * IFJ2022/project/error.h 
 * 
 * @brief Error handler interface declaration
 * 
 * @author Mazurava Maryia <xmazur08@vutbr.cz>
 */


#ifndef _ERROR_H
#define _ERROR_H

typedef enum error_types{
    NO_ERROR,
    LEXICAL_ERROR, 
    SYNTAX_ERROR,
    SEM_DEF_FUNC_ERROR, 
    SEM_TYPE_ERROR,
    SEM_FUNC_ERROR, 
    SEM_STMT_FUNC_ERROR, 
    SEM_TYPE_COMP_ERROR, 
    SEM_OTHER_ERROR, 
    INTERNAL_ERROR,
    ALLOCATION_ERROR
} error_types_e;

int error(int error_name);

#endif // _ERROR_H