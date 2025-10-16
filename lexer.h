#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Token types
typedef enum {
    TOKEN_KEYWORD,
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_SEPARATOR,
    TOKEN_STRING_LITERAL,
    TOKEN_COMMENT,
    TOKEN_EOF,
    TOKEN_UNKNOWN
} TokenType;

// Token structure
typedef struct {
    TokenType type;
    char lexeme[256];
    int line;
    int column;
} Token;

// Function declarations
void initLexer(const char* source);
Token getNextToken();
const char* getTokenTypeName(TokenType type);
int isKeyword(const char* str);
void printToken(Token token);

#endif
