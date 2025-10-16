#ifndef COMPILER_H
#define COMPILER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_TOKEN_LEN 100
#define MAX_TOKENS 1000
#define MAX_PRODUCTIONS 50
#define MAX_SYMBOLS 50
#define MAX_STACK 100
#define MAX_STATES 100

// Token types
typedef enum {
    TOKEN_ID,
    TOKEN_NUMBER,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_MULTIPLY,
    TOKEN_DIVIDE,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_ASSIGN,
    TOKEN_SEMICOLON,
    TOKEN_EOF,
    TOKEN_UNKNOWN,
    TOKEN_EPSILON
} CompilerTokenType;

// Token structure
typedef struct {
    CompilerTokenType type;
    char lexeme[MAX_TOKEN_LEN];
    int line;
    int column;
} Token;

// Production rule structure
typedef struct {
    char lhs[MAX_TOKEN_LEN];
    char rhs[MAX_PRODUCTIONS][MAX_TOKEN_LEN];
    int rhs_count;
} Production;

// Grammar structure
typedef struct {
    Production productions[MAX_PRODUCTIONS];
    int prod_count;
    char start_symbol[MAX_TOKEN_LEN];
    char terminals[MAX_SYMBOLS][MAX_TOKEN_LEN];
    int terminal_count;
    char non_terminals[MAX_SYMBOLS][MAX_TOKEN_LEN];
    int non_terminal_count;
} Grammar;

// First and Follow sets
typedef struct {
    char symbol[MAX_TOKEN_LEN];
    char first[MAX_SYMBOLS][MAX_TOKEN_LEN];
    int first_count;
    char follow[MAX_SYMBOLS][MAX_TOKEN_LEN];
    int follow_count;
} FirstFollowSet;

// LL Parsing Table
typedef struct {
    char non_terminal[MAX_TOKEN_LEN];
    char terminal[MAX_TOKEN_LEN];
    int production_index;
} LLTableEntry;

// LR Item
typedef struct {
    int production_index;
    int dot_position;
} LRItem;

// LR State
typedef struct {
    LRItem items[MAX_PRODUCTIONS];
    int item_count;
} LRState;

// LR Action
typedef enum {
    ACTION_SHIFT,
    ACTION_REDUCE,
    ACTION_ACCEPT,
    ACTION_ERROR
} LRActionType;

// LR Table Entry
typedef struct {
    LRActionType action;
    int state;  // for shift, or production number for reduce
} LRTableEntry;

// Function declarations for Lexer
void init_lexer(const char* input);
Token get_next_token();
Token peek_token();
const char* token_type_to_string(CompilerTokenType type);
void print_tokens();

// Function declarations for LL Parser
void init_ll_parser(Grammar* g);
void compute_first_sets(Grammar* g, FirstFollowSet first_sets[]);
void compute_follow_sets(Grammar* g, FirstFollowSet first_sets[], FirstFollowSet follow_sets[]);
void build_ll_table(Grammar* g, FirstFollowSet first_sets[], FirstFollowSet follow_sets[]);
bool parse_ll(const char* input);
void print_first_follow_sets(FirstFollowSet sets[], int count);
int get_ll_table_size();

// Function declarations for LR Parser
void init_lr_parser(Grammar* g);
void build_lr_items(Grammar* g);
void build_lr_table(Grammar* g);
bool parse_lr(const char* input);
void print_lr_table();

// Utility functions
void print_grammar(Grammar* g);
void load_sample_grammar(Grammar* g, int choice);
bool is_terminal(const char* symbol, Grammar* g);
bool is_non_terminal(const char* symbol, Grammar* g);

#endif // COMPILER_H
