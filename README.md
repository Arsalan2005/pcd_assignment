# pcd_assignment
## Lexical Analyzer in C

A simple lexical analyzer (lexer/tokenizer) implemented in C language that recognizes and categorizes tokens from C source code.

### Features

The lexical analyzer can identify the following token types:
- **Keywords**: C keywords like `int`, `float`, `if`, `for`, `while`, `return`, etc.
- **Identifiers**: Variable names, function names
- **Numbers**: Integer and floating-point literals
- **Operators**: Arithmetic (+, -, *, /, %), relational (<, >, <=, >=, ==, !=), logical (&&, ||), etc.
- **Separators**: Parentheses, braces, brackets, semicolons, commas, etc.
- **String Literals**: Text enclosed in double quotes
- **Character Literals**: Single characters enclosed in single quotes
- **Comments**: Both single-line (//) and multi-line (/* */) comments are handled

### Building the Project

To compile the lexical analyzer, use the provided Makefile:

```bash
make
```

To clean the build artifacts:

```bash
make clean
```

### Usage

#### Run with Default Sample Code

```bash
./lexer
```

This will analyze a built-in sample C code and display all tokens.

#### Run with Input File

```bash
./lexer <filename.c>
```

Example:
```bash
./lexer test_input.c
```

### Output Format

The lexer outputs tokens in the following format:

```
TOKEN TYPE           LEXEME               POSITION
-------------------------------------------------
KEYWORD              int                  (Line: 1, Col: 1)
IDENTIFIER           main                 (Line: 1, Col: 5)
SEPARATOR            (                    (Line: 1, Col: 9)
```

Each token shows:
- **Token Type**: The category of the token
- **Lexeme**: The actual text of the token
- **Position**: Line and column number where the token appears

### Files

- `lexer.h`: Header file with token definitions and function declarations
- `lexer.c`: Implementation of the lexical analyzer
- `main.c`: Main program to run the lexer
- `Makefile`: Build configuration
- `test_input.c`: Sample C file for testing

### Example

Given the input:
```c
int x = 10;
```

The lexer produces:
```
KEYWORD              int                  (Line: 1, Col: 1)
IDENTIFIER           x                    (Line: 1, Col: 5)
OPERATOR             =                    (Line: 1, Col: 7)
NUMBER               10                   (Line: 1, Col: 9)
SEPARATOR            ;                    (Line: 1, Col: 11)
```

