/*
 * IFJ2022/project/str.h
 *
 * @brief Dynamic string interface declaration
 *
 * @author Taipova Evgenia <xtaipo00@vutbr.cz>
 */

#ifndef STR_H
#define STR_H

// structure of dynamic string
typedef struct string
{
  char *str;     // place for string
  int length;    // length of string
  int allocSize; // allocated size
} string_t;

/*
 * @brief Initializes the string structure
 * @param *s Pointer to the string structure
 * @return STR_ERROR if the memory allocation fails, STR_SUCCESS otherwise
 */
int str_init(string_t *s);

/*
 * @brief Frees the string structure
 * @param *s Pointer to the string structure
 * @return void
 */
void str_free(string_t *s);

/*
 * @brief Adds a character to the end of the string
 * @param *s1 Pointer to the string structure, c Character to be added
 * @return STR_ERROR if the memory allocation fails, STR_SUCCESS otherwise
 */
int str_add_char(string_t *s1, char c);

/*
 * @brief Copies a string s2 to another string s1
 * @param *s1 Pointer to the string structure, *s2 Pointer to the string structure
 * @return STR_ERROR if the memory allocation fails, STR_SUCCESS otherwise
 */
int str_copy_string(string_t *s1, string_t *s2);

/*
 * @brief Compares a string s1 with a string s2
 * @param *s1 Pointer to the string structure, *s2 Pointer to the string structure
 * @return 0 if the strings are equal, 1 if the first string is greater than the second, -1 if the first string is less than the second
 */
int str_cmp_const_str(string_t *s1, const char *s2);

/*
 * @brief Concatenates a string s2 to the end of a string s1
 * @param *s1 Pointer to the string structure, *s2 Pointer to the string structure
 * @return STR_ERROR if the memory allocation fails, STR_SUCCESS otherwise
 */
int str_concat(string_t *s1, string_t *s2);

/*
 * @brief Compares a string s1 with a string s2
 * @param *s1 Pointer to the string structure, *s2 Pointer to the string structure
 * @return 0 if the strings are equal, 1 if the first string is greater than the second, -1 if the first string is less than the second
 */
int str_cmp_string(string_t *s1, string_t *s2);

#endif // STR_H