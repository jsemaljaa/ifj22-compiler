/*
 * IFJ2022/project/str.h 
 * 
 * @brief Dynamic string interface declaration
 * 
 * @author Taipova Evgenia <xtaipo00@vutbr.cz>
 */

#ifndef STR_H
#define STR_H 

typedef struct string{
  char* str;		  // misto pro dany retezec ukonceny znakem '\0'
  int length;		  // skutecna delka retezce
  int allocSize;	// velikost alokovane pameti
} string_t;


int str_init(string_t *s);
void str_print(string_t *s);
void str_free(string_t *s);

void str_clear(string_t *s);
int str_add_char(string_t *s1, char c);
int str_copy_string(string_t *s1, string_t *s2);
int str_cmp_string(string_t *s1, string_t *s2);
int str_cmp_const_str(string_t *s1, const char *s2);

char *str_get_str(string_t *s);
int str_get_length(string_t *s);

int str_concat(string_t *s1, string_t *s2);

#endif // STR_H