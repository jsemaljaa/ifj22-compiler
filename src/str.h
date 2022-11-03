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


int strInit(string_t *s);
void strFree(string_t *s);

void strClear(string_t *s);
int strAddChar(string_t *s1, char c);
int strCopyString(string_t *s1, string_t *s2);
int strCmpString(string_t *s1, string_t *s2);
int strCmpConstStr(string_t *s1, char *s2);

char *strGetStr(string_t *s);
int strGetLength(string_t *s);

#endif // STR_H