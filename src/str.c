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

// the constant STR_LEN_INC indicates how many bytes we will perform the initial allocation of memory
// if we load the string character by character,
// the memory will be gradually allocated to multiples of this number
#define STR_LEN_INC 8

#define STR_ERROR 1
#define STR_SUCCESS 0

int str_init(string_t *s)
{
   if ((s->str = (char *)malloc(STR_LEN_INC)) == NULL)
      return STR_ERROR;
   s->str[0] = '\0';
   s->length = 0;
   s->allocSize = STR_LEN_INC;
   return STR_SUCCESS;
}

void str_free(string_t *s)
{
   if (s->str != NULL)
   {
      free(s->str);
      s->str = NULL;
   }
}

int str_add_char(string_t *s1, char c)
{
   if (s1 == NULL || s1->str == NULL)
      return STR_ERROR;
   if (s1->length + 1 >= s1->allocSize)
   {
      s1->str = (char *)realloc(s1->str, s1->length + STR_LEN_INC);
      if (s1->str == NULL)
         return STR_ERROR;
      s1->allocSize = s1->length + STR_LEN_INC;
   }
   s1->str[s1->length++] = c;
   s1->str[s1->length] = '\0';
   return STR_SUCCESS;
}

int str_copy_string(string_t *s1, string_t *s2)
{
   int newLength = s2->length;
   if (newLength >= s1->allocSize)
   {
      if ((s1->str = (char *)realloc(s1->str, newLength + 1)) == NULL)
         return STR_ERROR;
      s1->allocSize = newLength + 1;
   }
   strcpy(s1->str, s2->str);
   s1->length = newLength;
   return STR_SUCCESS;
}

int str_cmp_const_str(string_t *s1, const char *s2)
{
   return strcmp(s1->str, s2);
}

int str_concat(string_t *s1, string_t *s2){
   if(s1 == NULL || s2 == NULL) return STR_ERROR;

   if(s1->allocSize <= s1->length + s2->length){
      s1->allocSize = s1->allocSize + s2->allocSize;
      if ((s1->str = (char*) realloc(s1->str, s1->allocSize + 1)) == NULL)
         return STR_ERROR;
   }

   strcat(s1->str, s2->str);
   s1->length = strlen(s1->str);

   return STR_SUCCESS;
}

int str_cmp_string(string_t *s1, string_t *s2)
{
   return strcmp(s1->str, s2->str);
}