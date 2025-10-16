#include "compiler.h"

static const char* input_string = NULL;
static int pos = 0;
static int line = 1;
static int column = 1;

void init_lexer(const char* input) {
    input_string = input;
    pos = 0;
    line = 1;
    column = 1;
}

static void skip_whitespace() {
    while (input_string[pos] != '\0' && isspace(input_string[pos])) {
        if (input_string[pos] == '\n') {
            line++;
            column = 1;
        } else {
            column++;
        }
        pos++;
    }
}

Token get_next_token() {
    Token token;
    token.line = line;
    token.column = column;
    memset(token.lexeme, 0, MAX_TOKEN_LEN);
    
    skip_whitespace();
    
    if (input_string[pos] == '\0') {
        token.type = TOKEN_EOF;
        strcpy(token.lexeme, "$");
        return token;
    }
    
    // Identifiers and keywords
    if (isalpha(input_string[pos]) || input_string[pos] == '_') {
        int i = 0;
        while ((isalnum(input_string[pos]) || input_string[pos] == '_') && i < MAX_TOKEN_LEN - 1) {
            token.lexeme[i++] = input_string[pos++];
            column++;
        }
        token.lexeme[i] = '\0';
        token.type = TOKEN_ID;
        return token;
    }
    
    // Numbers
    if (isdigit(input_string[pos])) {
        int i = 0;
        while (isdigit(input_string[pos]) && i < MAX_TOKEN_LEN - 1) {
            token.lexeme[i++] = input_string[pos++];
            column++;
        }
        token.lexeme[i] = '\0';
        token.type = TOKEN_NUMBER;
        return token;
    }
    
    // Operators and symbols
    char ch = input_string[pos];
    token.lexeme[0] = ch;
    token.lexeme[1] = '\0';
    pos++;
    column++;
    
    switch (ch) {
        case '+':
            token.type = TOKEN_PLUS;
            break;
        case '-':
            token.type = TOKEN_MINUS;
            break;
        case '*':
            token.type = TOKEN_MULTIPLY;
            break;
        case '/':
            token.type = TOKEN_DIVIDE;
            break;
        case '(':
            token.type = TOKEN_LPAREN;
            break;
        case ')':
            token.type = TOKEN_RPAREN;
            break;
        case '=':
            token.type = TOKEN_ASSIGN;
            break;
        case ';':
            token.type = TOKEN_SEMICOLON;
            break;
        default:
            token.type = TOKEN_UNKNOWN;
            break;
    }
    
    return token;
}

Token peek_token() {
    int saved_pos = pos;
    int saved_line = line;
    int saved_column = column;
    
    Token token = get_next_token();
    
    pos = saved_pos;
    line = saved_line;
    column = saved_column;
    
    return token;
}

const char* token_type_to_string(CompilerTokenType type) {
    switch (type) {
        case TOKEN_ID: return "IDENTIFIER";
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_PLUS: return "PLUS";
        case TOKEN_MINUS: return "MINUS";
        case TOKEN_MULTIPLY: return "MULTIPLY";
        case TOKEN_DIVIDE: return "DIVIDE";
        case TOKEN_LPAREN: return "LPAREN";
        case TOKEN_RPAREN: return "RPAREN";
        case TOKEN_ASSIGN: return "ASSIGN";
        case TOKEN_SEMICOLON: return "SEMICOLON";
        case TOKEN_EOF: return "EOF";
        case TOKEN_EPSILON: return "EPSILON";
        default: return "UNKNOWN";
    }
}

void print_tokens() {
    printf("\n=== LEXICAL ANALYSIS ===\n");
    printf("%-15s %-20s %-10s %-10s\n", "TOKEN TYPE", "LEXEME", "LINE", "COLUMN");
    printf("---------------------------------------------------------------\n");
    
    int saved_pos = pos;
    int saved_line = line;
    int saved_column = column;
    
    init_lexer(input_string);
    
    Token token;
    do {
        token = get_next_token();
        printf("%-15s %-20s %-10d %-10d\n", 
               token_type_to_string(token.type), 
               token.lexeme, 
               token.line, 
               token.column);
    } while (token.type != TOKEN_EOF);
    
    pos = saved_pos;
    line = saved_line;
    column = saved_column;
}
