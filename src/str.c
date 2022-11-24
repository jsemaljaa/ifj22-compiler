/*
 * IFJ2022/project/str.h 
 * 
 * @brief Dynamic string interface implementation
 * 
 * @author Taipova Evgenia <xtaipo00@vutbr.cz>
 */


#include <string.h>
#include <malloc.h>
#include "str.h"

#define STR_LEN_INC 8
// konstanta STR_LEN_INC udava, na kolik bytu provedeme pocatecni alokaci pameti
// pokud nacitame retezec znak po znaku, pamet se postupne bude alkokovat na
// nasobky tohoto cisla 

#define STR_ERROR   1
#define STR_SUCCESS 0

int str_init(string_t *s)
// funkce vytvori novy retezec
{
   if ((s->str = (char*) malloc(STR_LEN_INC)) == NULL) 
      return STR_ERROR;
   s->str[0] = '\0';
   s->length = 0;
   s->allocSize = STR_LEN_INC;
   return STR_SUCCESS;
}

void str_print(string_t *s)
{
   printf("-- BEGIN DEBUG: STRING INFO\n");
   printf("string: %s\n", s->str);
   printf("length: %d\n", s->length);
   printf("allocation size: %d\n", s->allocSize);
   printf("-- END DEBUG: STRING INFO\n");
} 

void str_free(string_t *s)
// funkce uvolni retezec z pameti
{
   if(s->str != NULL){
      free(s->str);
      s->str = NULL;
   }
}

void str_clear(string_t *s)
// funkce vymaze obsah retezce
{
   s->str[0] = '\0';
   s->length = 0;
}

int str_add_char(string_t *s1, char c)
// prida na konec retezce jeden znak
{
   if(s1 == NULL || s1->str == NULL) return STR_ERROR;
   if (s1->length + 1 >= s1->allocSize)
   {
      // pamet nestaci, je potreba provest realokaci
      s1->str = (char*) realloc(s1->str, s1->length + STR_LEN_INC);
      if(s1->str == NULL) return STR_ERROR;
      s1->allocSize = s1->length + STR_LEN_INC;
   }
   s1->str[s1->length++] = c;
   s1->str[s1->length] = '\0';
   return STR_SUCCESS;
}

int str_copy_string(string_t *s1, string_t *s2)
// prekopiruje retezec s2 do s1
{
   int newLength = s2->length;
   if (newLength >= s1->allocSize)
   {
      // pamet nestaci, je potreba provest realokaci
      if ((s1->str = (char*) realloc(s1->str, newLength + 1)) == NULL)
         return STR_ERROR;
      s1->allocSize = newLength + 1;
   }
   strcpy(s1->str, s2->str);
   s1->length = newLength;
   return STR_SUCCESS;
}

int str_cmp_string(string_t *s1, string_t *s2)
// porovna oba retezce a vrati vysledek
{
   return strcmp(s1->str, s2->str);
}

int str_cmp_const_str(string_t *s1, const char* s2)
// porovna nas retezec s konstantnim retezcem
{
   return strcmp(s1->str, s2);
}

char *str_get_str(string_t *s)
// vrati textovou cast retezce
{
   return s->str;
}

int str_get_length(string_t *s)
// vrati delku daneho retezce
{
   return s->length;
}