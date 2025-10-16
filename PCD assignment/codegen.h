#ifndef CODEGEN_H
#define CODEGEN_H

#include "compiler.h"

#define MAX_CODE_LINES 500
#define MAX_TEMPS 100

// Three-address code instruction
typedef struct {
    char op[20];
    char arg1[50];
    char arg2[50];
    char result[50];
} TAC;

// Code generator state
typedef struct {
    TAC instructions[MAX_CODE_LINES];
    int instr_count;
    int temp_count;
    int label_count;
} CodeGenerator;

// Function declarations
void init_codegen();
char* new_temp();
char* new_label();
void emit(const char* op, const char* arg1, const char* arg2, const char* result);
void generate_code_from_tokens();
char* generate_assembly();
void print_tac();

#endif // CODEGEN_H
