# Lexical Analyzer Implementation Details

## Overview
This is a complete implementation of a lexical analyzer (also known as a lexer, scanner, or tokenizer) written in C language. The lexer reads C source code and breaks it down into meaningful tokens.

## Components

### 1. Token Types (lexer.h)
The lexer recognizes the following token types:
- **TOKEN_KEYWORD**: Reserved words in C (int, float, if, for, while, etc.)
- **TOKEN_IDENTIFIER**: Variable and function names
- **TOKEN_NUMBER**: Integer and floating-point literals
- **TOKEN_OPERATOR**: Arithmetic, relational, logical, and assignment operators
- **TOKEN_SEPARATOR**: Parentheses, braces, brackets, semicolons, commas
- **TOKEN_STRING_LITERAL**: String and character literals
- **TOKEN_COMMENT**: Comments (automatically skipped)
- **TOKEN_EOF**: End of file marker
- **TOKEN_UNKNOWN**: Unrecognized characters

### 2. Token Structure
Each token contains:
```c
typedef struct {
    TokenType type;      // The category of the token
    char lexeme[256];    // The actual text
    int line;            // Line number in source
    int column;          // Column number in source
} Token;
```

### 3. Core Functions (lexer.c)

#### Initialization
- `initLexer(const char* source)`: Initializes the lexer with source code

#### Token Recognition
- `getNextToken()`: Main function that returns the next token from the source
- `isKeyword(const char* str)`: Checks if a string is a C keyword

#### Helper Functions
- `peek()`: Look at current character without consuming it
- `advance()`: Get current character and move to next
- `skipWhitespace()`: Skip spaces, tabs, newlines
- `readIdentifier()`: Read an identifier or keyword
- `readNumber()`: Read a numeric literal
- `readString()`: Read a string literal
- `readChar()`: Read a character literal
- `skipLineComment()`: Skip single-line comments (//)
- `skipBlockComment()`: Skip multi-line comments (/* */)

#### Output Functions
- `getTokenTypeName(TokenType type)`: Convert token type to string
- `printToken(Token token)`: Display token information

### 4. Main Program (main.c)
- Accepts input file as command-line argument or uses built-in sample code
- Initializes the lexer with the source code
- Continuously calls `getNextToken()` until EOF
- Prints formatted output showing all tokens with their positions

## Recognition Patterns

### Keywords
The lexer recognizes 32 C keywords including:
- Data types: int, float, char, double, void, etc.
- Control flow: if, else, for, while, do, switch, case, break, continue, return
- Storage classes: auto, static, extern, register
- Type qualifiers: const, volatile, signed, unsigned

### Identifiers
- Start with a letter (a-z, A-Z) or underscore (_)
- Followed by letters, digits, or underscores
- Examples: `variable`, `_temp`, `count123`

### Numbers
- Integer literals: `10`, `0`, `999`
- Floating-point literals: `3.14`, `0.5`, `2.718`

### Operators
- Arithmetic: `+`, `-`, `*`, `/`, `%`
- Relational: `<`, `>`, `<=`, `>=`, `==`, `!=`
- Logical: `&&`, `||`
- Increment/Decrement: `++`, `--`
- Assignment: `=`
- Bitwise: `&`, `|`, `^`, `~`, `<<`, `>>`

### Separators
- Parentheses: `(`, `)`
- Braces: `{`, `}`
- Brackets: `[`, `]`
- Semicolon: `;`
- Comma: `,`
- Dot: `.`
- Colon: `:`

### String Literals
- Enclosed in double quotes: `"Hello, World!"`
- Handles escape sequences: `"Line 1\nLine 2"`

### Character Literals
- Enclosed in single quotes: `'A'`, `'x'`
- Handles escape sequences: `'\n'`, `'\t'`

### Comments
- Single-line: `// comment`
- Multi-line: `/* comment */`
- Comments are automatically skipped (not returned as tokens)

## Usage Examples

### Example 1: Analyze a File
```bash
./lexer test_input.c
```

### Example 2: Use Default Sample
```bash
./lexer
```

### Example 3: Build from Source
```bash
make clean
make
./lexer comprehensive_test.c
```

## Limitations and Future Enhancements

### Current Limitations
1. Preprocessor directives (like `#include`) are not fully handled
2. No error recovery mechanism for malformed input
3. Fixed-size lexeme buffer (256 characters)
4. Basic escape sequence handling

### Potential Enhancements
1. Add support for preprocessor directives
2. Implement error recovery and better error messages
3. Support for hexadecimal and octal numbers
4. Support for scientific notation (e.g., 1.5e10)
5. Dynamic memory allocation for lexemes
6. Symbol table integration
7. More comprehensive escape sequence handling
8. Unicode support

## Testing

The repository includes several test files:
1. **test_input.c**: Basic test with common C constructs
2. **comprehensive_test.c**: Extensive test covering all token types

## Building and Running

```bash
# Build
make

# Run with default code
./lexer

# Run with input file
./lexer test_input.c

# Clean build artifacts
make clean
```

## Technical Details

### Lexer State
The lexer maintains:
- Current position in source code
- Current line number
- Current column number

### Tokenization Algorithm
1. Skip whitespace
2. Check for end of file
3. Identify token type based on first character:
   - Letter/underscore → Identifier or keyword
   - Digit → Number
   - Quote → String/character literal
   - Operator character → Operator (check for multi-character)
   - Separator character → Separator
   - Other → Unknown token
4. Read complete token
5. Return token with type, lexeme, and position

This lexical analyzer serves as a foundation for building a complete compiler or interpreter for the C language.
