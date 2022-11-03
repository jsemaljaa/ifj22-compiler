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


int error(int error_name) {
    int error_number = 0;
    switch (error_name) {
        case LEXICAL_ERROR:
            fprintf(stderr, "%s\n", "1 -  chyba v programu v ramci lexikalni analyzy.\n");
            error_number = 1;
            break;
        case SYNTAX_ERROR:
            fprintf(stderr, "%s\n", "2 - chyba v programu v ramci syntakticke analyzy.\n");
            error_number = 2;
            break;
        case SEM_DEF_FUNC_ERROR:
            fprintf(stderr, "%s\n", "3 - semanticka chyba v programu – nedefinovaná funkce/promenna, pokus o redefinici funkce/promenne, atp.\n");
            error_number = 3;
            break;
        case SEM_TYPE_ERROR:
            fprintf(stderr, "%s\n", "4 - semanticka/behova chyba typové kompatibility v aritmetickych, retezcovych a relacnich vyrazech.\n");
            error_number = 4;
            break;
        case SEM_FUNC_ERROR:
            fprintf(stderr, "%s\n", "5 - semanticka chyba v programu – spatny pocet parametru u volani funkce.\n");
            error_number = 5;
            break;
        case SEM_STMT_FUNC_ERROR:
            fprintf(stderr, "%s\n", "6 - ostatni semanticke chyby.\n");
            error_number = 6;
            break;
        case SEM_TYPE_COMP_ERROR:
            fprintf(stderr, "%s\n", "9 - behova chyba deleni nulou.\n");
            error_number = 7;
            break;
        case SEM_OTHER_ERROR:
            fprintf(stderr, "%s\n", "9 - behova chyba deleni nulou.\n");
            error_number = 8;
            break;
        case INTERNAL_ERROR:
            fprintf(stderr, "%s\n", "99 - interni chyba prekladace tj. neovlivnena vstupnim programem (napr. chyba alokace pameti, atd.).\n");
            error_number = 99;
            break;
        case ALLOCATION_ERROR:
            fprintf(stderr, "%s\n", "99 - interni chyba prekladace tj. neovlivnena vstupnim programem (napr. chyba alokace pameti, atd.).\n");
            error_number = 99;
            break;
    }

    
//while (!stack_sem_empty(&stack_semantic))
       // stack_sem_pop(&stack_semantic);

    
    symt_free(&symtable);
    return error_number;
}