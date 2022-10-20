// hlavicka pro lexikalni analyzator
#ifndef SCANNER_H
#define SCANNER_H

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

// identifikator
#define ID 0

// klicova slova
#define ELSE 10
#define FLOAT 11
#define FUNCTION 12
#define IF 13
#define INT 14
#define KEYWORD_NULL        15
#define RETURN 16
#define STRING 17
#define VOID 18
#define WHILE 19

// operatory
#define PLUS 20       // +
#define MINUS 21      // -
#define MUL 22        // *
#define DIV 23        // /
#define EQUAL 24      // ===
#define NOT_EQUAL 25  // !==
#define LESS 26       // <
#define GREATER 27    // >
#define LESS_EQ 28    // <=
#define GREATER_EQ 29 // >=

// jednotlive znaky
#define LEFT_PAR 30  // '('
#define RIGHT_PAR 31 // ')'
#define SEMICOLON 32 // ';'
#define COLON 33     // ':'
#define ASSIGN 34    // '='
#define COMMA 35     // ','
#define POINT 36     // '.'
#define LEFT_BR 37   // '{'
#define RIGHT_BR 38  // '}'
#define DO_QUOTES 39 // '"'

#define TYPE_INT 40
#define TYPE_FLOAT 41
#define TYPE_STRING 42

// specialni znaky
#define END_OF_FILE 50
#define END_OF_LINE 51

// chybove hlasky
#define LEX_ERROR 1

// Scanner states.
#define STATE_START 200                  /// Starting state every new token processing starts here and initializes other states
#define STATE_BACKSLASH 202              /// Operator / OR start of (block) comment - next symbol must be (*) /
#define STATE_COMMENTARY 201             /// Starts with //, ignores every symbol, ends with EOL or EOF
#define STATE_BLOCK_COMMENTARY 203       /// Starts with /* and ignores every symbol except * - this might be the end of block comment
#define STATE_BLOCK_COMMENTARY_LEAVE 204 /// Ends with */ the * is read, if the next symbol is /, leave, else if *, stay, else go back to state before
#define STATE_IDENTIFIER_OR_KEYWORD 205  /// Starts with letter or _, if next symbols are alphanumeric or _, add them to string, which is later compared with reserved words | Returns either keyword or string as ID

#define STATE_EQUAL_START 206           /// Starts with =, if next symbol is =, return EQUAL, else return ASSIGN
#define STATE_EQUAL 207                 /// Starts with =, if next symbol is =, return EQUAL, else return ASSIGN
#define STATE_NOT_EQUAL 208             /// Starts with !, if next symbol is =, return NOT_EQUAL, else return LEX_ERROR
#define STATE_NOT_EQUAL_START 209       /// Starts with !, if next symbol is =, return NOT_EQUAL, else return LEX_ERROR
#define STATE_DIV 210                   /// Starts with /, if next symbol is /, return DIV, else return LEX_ERROR
#define STATE_NUMBER 211                /// Start of number processing, accepts numbers, e/E and . | Can return integer number
#define STATE_NUMBER_POINT 212          /// If symbol was ., the number has type double
#define STATE_NUMBER_DOUBLE 213         /// The last symbol was number | Can return double number
#define STATE_NUMBER_EXPONENT 214       /// The last symbol was e or E, the number has type double, continues with optional symbols +/- or number
#define STATE_NUMBER_EXPONENT_SIGN 215  /// Optional symbol was read, continue with numbers only
#define STATE_NUMBER_EXPONENT_FINAL 216 /// Returns double number with exponent

#define STATE_STRING_START 217  /// String starts with !" else returns error
#define STATE_STRING 218        /// Sequence !" was read, ends with ", if ASCII value is lower than 32, returns error, these symbols can be written using escape sequence | Returns string
#define STATE_STRING_ESCAPE 219 /// If symbol \ was loaded, can replace char with escape sequence symbols
#define STATE_STRING_ESCAPE_X 220
#define STATE_STRING_ESCAPE_X_ZERO 221
#define STATE_STRING_ESCAPE_X_NOT_ZERO 222
#define STATE_STRING_ESCAPE_ZERO 223      /// 0 loaded, accepts only digits
#define STATE_STRING_ESCAPE_ZERO_ZERO 224 /// 00 loaded, accepts only digits
#define STATE_STRING_ESCAPE_ONE_THREE 225 /// first 2 then 5 was loaded, accepts only 0 to 5 digits, returns symbol with ASCII value
#define STATE_STRING_ESCAPE_ONE_THREE_REST 226
#define STATE_STRING_ESCAPE_REST 227 /// the rest cases, when the third number can be only digit, from 001 to 249
#define STATE_LESS_THAN 228          /// Starts with < | Returns <= or <
#define STATE_MORE_THAN 229          /// Starts with > | Returns > or >=

// hlavicka funkce simulujici lexikalni analyzator
void setSourceFile(FILE *f);
int getNextToken(string *attr);

#endif /* SCANNER_H*/