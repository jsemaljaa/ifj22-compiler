# IFJ22 Imperative Language Compiler Implementation 

## Project from [IFJ](https://www.fit.vut.cz/study/course/IFJ/.en) university course 2022/23

The goal of the project was to write a console application (a compiler, in C) for the custom language IFJ22.   

Our team developed the following structure for the final implementation:
1. Scanner (lexical analysis of the input file)
2. Parser (syntactic and semantic analysis)
3. Code generator (generates machine code in IFJcode22)

## Implementation details

### Scanner
This module is the first to be started and is responsible for lexical analysis of the read file and subsequent transfer of the tokenized data to the next module (first of all, to the parser module). It is implemented in the file `scanner.c` (also its auxiliary module `str.c`) according to the designed finite state machine (page 5 in the project documentation). The entire FSM is represented by a single switch statement, where one case corresponds to one state. With these state the scanner constructs and passes the tokens on to the parser module. 

### Parser
This module (implemented in files `parser.c`, `expressions.c`, and auxiliary data structure modules in files `symstack.c` and `symtable.c`) is the main and central module of the entire application. Here, the syntactic and semantic analysis of the tokens coming from the scanner is performed, which then, after successful analysis, go to the generator module. The parser is based on the LL grammar whose rules were created by our team (pages 6 and 7 of the project documentation). For semantic analysis we used a precedence table (page 8 of the project documentation), which is implemented in the auxiliary module `expressions.c`

### Code generator 
This module generates code in the target language IFJcode22. Runs and works in parallel with the parser, and in the process writes instructions to the standard output in accordance with the input data. Implemented in file `generator.c`.

## Usage
Compile the entire application:
```bash
$ make
```

Run:
```bash
$ ./ifj22 <input
```
where input is file containing IFJ22 code

## Example
Given file `example1.php`: 
```php
<?php
declare(strict_types=1);
// Program 1: Vypocet faktorialu (iterativne)

// Hlavni telo programu
write("Zadejte cislo pro vypocet faktorialu\n");
$a = readi();
if ($a === null) {
    write("Chyba pri nacitani celeho cisla!\n");
} else {}
if ($a < 0) {
    write("Faktorial nelze spocitat\n");
} else {
    $vysl = 1;
    while ($a > 0) {
        $vysl = $vysl * $a;
        $a = $a - 1;
    }
    write("Vysledek je: ", $vysl, "\n");
}
```
After running:
```bash
./ifj22 <example1.php
```
Outputs:
```
.IFJcode22
CREATEFRAME
PUSHFRAME
DEFVAR LF@$arg0
MOVE LF@$arg0 string@Zadejte\032cislo\032pro\032vypocet\032faktorialu\010
WRITE LF@$arg0
LABEL !readi
PUSHFRAME
DEFVAR LF@$readi
READ LF@$readi int
LABEL !end_readi
POPFRAME
RETURN
JUMPIFEQ
LABEL !if_func1
PUSHFRAME
DEFVAR LF@$arg1
MOVE LF@$arg1 string@Chyba\032pri\032nacitani\032celeho\032cisla!\010
WRITE LF@$arg1
LABEL !end_if_func1
POPFRAME
RETURN
LABEL !else_func1
PUSHFRAME
LABEL !end_else_func1
POPFRAME
RETURN
JUMPIFEQ
LABEL !if_func2
PUSHFRAME
DEFVAR LF@$arg2
MOVE LF@$arg2 string@Faktorial\032nelze\032spocitat\010
WRITE LF@$arg2
LABEL !end_if_func2
POPFRAME
RETURN
LABEL !else_func2
PUSHFRAME
LABEL !while_func1
PUSHFRAME
DEFVAR LF@$loop1
MOVE LF@$loop1 bool@true
LABEL !end_while_func1
POPFRAME
RETURN
DEFVAR LF@$arg3
MOVE LF@$arg3 string@Vysledek\032je:\032
WRITE LF@$arg3
DEFVAR LF@$arg4
DEFVAR LF@$arg4
MOVE LF@$arg4 nil@nil
WRITE LF@$arg4
DEFVAR LF@$arg5
MOVE LF@$arg5 string@\010
WRITE LF@$arg5
LABEL !end_else_func2
POPFRAME
RETURN
```
## Final evaluation

- Lexical analysis: **96%**
- Syntactic analysis: **56%**
- Semantic analysis: **28%**
- Code generator: **3%**

### Total Score: 16.32/60.00
(distributed equally among 4 team members)

## Authors:
[Alina Vinogradova](https://github.com/jsemaljaa)
[Ekaterina Krupenko](https://github.com/sirotka)
[Maryia Mazurava](https://github.com/maryia-mazurava)
[Evgenia Taipova](https://github.com/evgenia-taipova)    

[Tests (by @galloj)](https://github.com/galloj/IFJ22_Tester)
