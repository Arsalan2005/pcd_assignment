#include "lexer.h"

// Global variables for lexer state
static const char* source = NULL;
static int position = 0;
static int line = 1;
static int column = 1;

// C Keywords
const char* keywords[] = {
    "auto", "break", "case", "char", "const", "continue", "default", "do",
    "double", "else", "enum", "extern", "float", "for", "goto", "if",
    "int", "long", "register", "return", "short", "signed", "sizeof", "static",
    "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while"
};

const int NUM_KEYWORDS = sizeof(keywords) / sizeof(keywords[0]);

// Initialize the lexer with source code
void initLexer(const char* src) {
    source = src;
    position = 0;
    line = 1;
    column = 1;
}

// Check if a string is a keyword
int isKeyword(const char* str) {
    for (int i = 0; i < NUM_KEYWORDS; i++) {
        if (strcmp(str, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

// Get the current character without consuming it
static char peek() {
    if (source[position] == '\0') {
        return '\0';
    }
    return source[position];
}

// Get the current character and advance to the next
static char advance() {
    if (source[position] == '\0') {
        return '\0';
    }
    char c = source[position++];
    if (c == '\n') {
        line++;
        column = 1;
    } else {
        column++;
    }
    return c;
}

// Skip whitespace
static void skipWhitespace() {
    while (isspace(peek())) {
        advance();
    }
}

// Read an identifier or keyword
static Token readIdentifier() {
    Token token;
    token.line = line;
    token.column = column;
    int i = 0;
    
    while (isalnum(peek()) || peek() == '_') {
        token.lexeme[i++] = advance();
    }
    token.lexeme[i] = '\0';
    
    if (isKeyword(token.lexeme)) {
        token.type = TOKEN_KEYWORD;
    } else {
        token.type = TOKEN_IDENTIFIER;
    }
    
    return token;
}

// Read a number
static Token readNumber() {
    Token token;
    token.type = TOKEN_NUMBER;
    token.line = line;
    token.column = column;
    int i = 0;
    
    while (isdigit(peek()) || peek() == '.') {
        token.lexeme[i++] = advance();
    }
    token.lexeme[i] = '\0';
    
    return token;
}

// Read a string literal
static Token readString() {
    Token token;
    token.type = TOKEN_STRING_LITERAL;
    token.line = line;
    token.column = column;
    int i = 0;
    
    token.lexeme[i++] = advance(); // Opening quote
    
    while (peek() != '"' && peek() != '\0') {
        if (peek() == '\\') {
            token.lexeme[i++] = advance();
            if (peek() != '\0') {
                token.lexeme[i++] = advance();
            }
        } else {
            token.lexeme[i++] = advance();
        }
    }
    
    if (peek() == '"') {
        token.lexeme[i++] = advance(); // Closing quote
    }
    token.lexeme[i] = '\0';
    
    return token;
}

// Read a character literal
static Token readChar() {
    Token token;
    token.type = TOKEN_STRING_LITERAL;
    token.line = line;
    token.column = column;
    int i = 0;
    
    token.lexeme[i++] = advance(); // Opening quote
    
    while (peek() != '\'' && peek() != '\0') {
        if (peek() == '\\') {
            token.lexeme[i++] = advance();
            if (peek() != '\0') {
                token.lexeme[i++] = advance();
            }
        } else {
            token.lexeme[i++] = advance();
        }
    }
    
    if (peek() == '\'') {
        token.lexeme[i++] = advance(); // Closing quote
    }
    token.lexeme[i] = '\0';
    
    return token;
}

// Skip single-line comment
static void skipLineComment() {
    while (peek() != '\n' && peek() != '\0') {
        advance();
    }
}

// Skip multi-line comment
static void skipBlockComment() {
    advance(); // Skip '*'
    while (1) {
        if (peek() == '\0') {
            break;
        }
        if (peek() == '*') {
            advance();
            if (peek() == '/') {
                advance();
                break;
            }
        } else {
            advance();
        }
    }
}

// Get the next token
Token getNextToken() {
    skipWhitespace();
    
    Token token;
    token.line = line;
    token.column = column;
    
    char c = peek();
    
    // End of file
    if (c == '\0') {
        token.type = TOKEN_EOF;
        strcpy(token.lexeme, "EOF");
        return token;
    }
    
    // Identifiers and keywords
    if (isalpha(c) || c == '_') {
        return readIdentifier();
    }
    
    // Numbers
    if (isdigit(c)) {
        return readNumber();
    }
    
    // String literals
    if (c == '"') {
        return readString();
    }
    
    // Character literals
    if (c == '\'') {
        return readChar();
    }
    
    // Comments
    if (c == '/') {
        advance();
        if (peek() == '/') {
            advance();
            skipLineComment();
            return getNextToken();
        } else if (peek() == '*') {
            skipBlockComment();
            return getNextToken();
        } else {
            token.type = TOKEN_OPERATOR;
            strcpy(token.lexeme, "/");
            return token;
        }
    }
    
    // Operators and separators
    token.line = line;
    token.column = column;
    
    // Multi-character operators
    if (c == '=' || c == '!' || c == '<' || c == '>' || c == '&' || c == '|' || c == '+' || c == '-') {
        token.lexeme[0] = advance();
        if ((token.lexeme[0] == '=' && peek() == '=') ||
            (token.lexeme[0] == '!' && peek() == '=') ||
            (token.lexeme[0] == '<' && peek() == '=') ||
            (token.lexeme[0] == '>' && peek() == '=') ||
            (token.lexeme[0] == '&' && peek() == '&') ||
            (token.lexeme[0] == '|' && peek() == '|') ||
            (token.lexeme[0] == '+' && peek() == '+') ||
            (token.lexeme[0] == '-' && peek() == '-') ||
            (token.lexeme[0] == '<' && peek() == '<') ||
            (token.lexeme[0] == '>' && peek() == '>')) {
            token.lexeme[1] = advance();
            token.lexeme[2] = '\0';
        } else {
            token.lexeme[1] = '\0';
        }
        token.type = TOKEN_OPERATOR;
        return token;
    }
    
    // Single-character operators
    if (c == '*' || c == '%' || c == '^' || c == '~') {
        token.type = TOKEN_OPERATOR;
        token.lexeme[0] = advance();
        token.lexeme[1] = '\0';
        return token;
    }
    
    // Separators
    if (c == '(' || c == ')' || c == '{' || c == '}' || c == '[' || c == ']' ||
        c == ';' || c == ',' || c == '.' || c == ':') {
        token.type = TOKEN_SEPARATOR;
        token.lexeme[0] = advance();
        token.lexeme[1] = '\0';
        return token;
    }
    
    // Unknown character
    token.type = TOKEN_UNKNOWN;
    token.lexeme[0] = advance();
    token.lexeme[1] = '\0';
    return token;
}

// Get token type name
const char* getTokenTypeName(TokenType type) {
    switch (type) {
        case TOKEN_KEYWORD: return "KEYWORD";
        case TOKEN_IDENTIFIER: return "IDENTIFIER";
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_OPERATOR: return "OPERATOR";
        case TOKEN_SEPARATOR: return "SEPARATOR";
        case TOKEN_STRING_LITERAL: return "STRING_LITERAL";
        case TOKEN_COMMENT: return "COMMENT";
        case TOKEN_EOF: return "EOF";
        case TOKEN_UNKNOWN: return "UNKNOWN";
        default: return "UNKNOWN";
    }
}

// Print token information
void printToken(Token token) {
    printf("%-20s %-20s (Line: %d, Col: %d)\n", 
           getTokenTypeName(token.type), 
           token.lexeme, 
           token.line, 
           token.column);
}
