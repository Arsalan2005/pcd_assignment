#include "compiler.h"

bool is_terminal(const char* symbol, Grammar* g) {
    for (int i = 0; i < g->terminal_count; i++) {
        if (strcmp(g->terminals[i], symbol) == 0) {
            return true;
        }
    }
    return false;
}

bool is_non_terminal(const char* symbol, Grammar* g) {
    for (int i = 0; i < g->non_terminal_count; i++) {
        if (strcmp(g->non_terminals[i], symbol) == 0) {
            return true;
        }
    }
    return false;
}

void print_grammar(Grammar* g) {
    printf("\n=== GRAMMAR ===\n");
    printf("Start Symbol: %s\n\n", g->start_symbol);
    
    printf("Productions:\n");
    for (int i = 0; i < g->prod_count; i++) {
        printf("%d. %s ->", i + 1, g->productions[i].lhs);
        for (int j = 0; j < g->productions[i].rhs_count; j++) {
            printf(" %s", g->productions[i].rhs[j]);
        }
        printf("\n");
    }
    
    printf("\nTerminals: { ");
    for (int i = 0; i < g->terminal_count; i++) {
        printf("%s", g->terminals[i]);
        if (i < g->terminal_count - 1) printf(", ");
    }
    printf(" }\n");
    
    printf("Non-Terminals: { ");
    for (int i = 0; i < g->non_terminal_count; i++) {
        printf("%s", g->non_terminals[i]);
        if (i < g->non_terminal_count - 1) printf(", ");
    }
    printf(" }\n");
}

void load_sample_grammar(Grammar* g, int choice) {
    g->prod_count = 0;
    g->terminal_count = 0;
    g->non_terminal_count = 0;
    
    switch (choice) {
        case 1: {
            // Simple arithmetic expression grammar
            // E -> E + T | T
            // T -> T * F | F
            // F -> ( E ) | id
            
            strcpy(g->start_symbol, "E");
            
            // Production 1: E -> E + T
            strcpy(g->productions[0].lhs, "E");
            strcpy(g->productions[0].rhs[0], "E");
            strcpy(g->productions[0].rhs[1], "+");
            strcpy(g->productions[0].rhs[2], "T");
            g->productions[0].rhs_count = 3;
            
            // Production 2: E -> T
            strcpy(g->productions[1].lhs, "E");
            strcpy(g->productions[1].rhs[0], "T");
            g->productions[1].rhs_count = 1;
            
            // Production 3: T -> T * F
            strcpy(g->productions[2].lhs, "T");
            strcpy(g->productions[2].rhs[0], "T");
            strcpy(g->productions[2].rhs[1], "*");
            strcpy(g->productions[2].rhs[2], "F");
            g->productions[2].rhs_count = 3;
            
            // Production 4: T -> F
            strcpy(g->productions[3].lhs, "T");
            strcpy(g->productions[3].rhs[0], "F");
            g->productions[3].rhs_count = 1;
            
            // Production 5: F -> ( E )
            strcpy(g->productions[4].lhs, "F");
            strcpy(g->productions[4].rhs[0], "(");
            strcpy(g->productions[4].rhs[1], "E");
            strcpy(g->productions[4].rhs[2], ")");
            g->productions[4].rhs_count = 3;
            
            // Production 6: F -> id
            strcpy(g->productions[5].lhs, "F");
            strcpy(g->productions[5].rhs[0], "id");
            g->productions[5].rhs_count = 1;
            
            g->prod_count = 6;
            
            // Terminals
            strcpy(g->terminals[0], "id");
            strcpy(g->terminals[1], "+");
            strcpy(g->terminals[2], "*");
            strcpy(g->terminals[3], "(");
            strcpy(g->terminals[4], ")");
            strcpy(g->terminals[5], "$");
            g->terminal_count = 6;
            
            // Non-terminals
            strcpy(g->non_terminals[0], "E");
            strcpy(g->non_terminals[1], "T");
            strcpy(g->non_terminals[2], "F");
            g->non_terminal_count = 3;
            break;
        }
        
        case 2: {
            // Simple LL(1) grammar
            // S -> A B
            // A -> a | ε
            // B -> b
            
            strcpy(g->start_symbol, "S");
            
            // Production 1: S -> A B
            strcpy(g->productions[0].lhs, "S");
            strcpy(g->productions[0].rhs[0], "A");
            strcpy(g->productions[0].rhs[1], "B");
            g->productions[0].rhs_count = 2;
            
            // Production 2: A -> a
            strcpy(g->productions[1].lhs, "A");
            strcpy(g->productions[1].rhs[0], "a");
            g->productions[1].rhs_count = 1;
            
            // Production 3: A -> ε
            strcpy(g->productions[2].lhs, "A");
            strcpy(g->productions[2].rhs[0], "ε");
            g->productions[2].rhs_count = 1;
            
            // Production 4: B -> b
            strcpy(g->productions[3].lhs, "B");
            strcpy(g->productions[3].rhs[0], "b");
            g->productions[3].rhs_count = 1;
            
            g->prod_count = 4;
            
            // Terminals
            strcpy(g->terminals[0], "a");
            strcpy(g->terminals[1], "b");
            strcpy(g->terminals[2], "$");
            g->terminal_count = 3;
            
            // Non-terminals
            strcpy(g->non_terminals[0], "S");
            strcpy(g->non_terminals[1], "A");
            strcpy(g->non_terminals[2], "B");
            g->non_terminal_count = 3;
            break;
        }
        
        case 3: {
            // Another simple grammar
            // E -> T E'
            // E' -> + T E' | ε
            // T -> F T'
            // T' -> * F T' | ε
            // F -> ( E ) | id
            
            strcpy(g->start_symbol, "E");
            
            // Production 1: E -> T E'
            strcpy(g->productions[0].lhs, "E");
            strcpy(g->productions[0].rhs[0], "T");
            strcpy(g->productions[0].rhs[1], "E'");
            g->productions[0].rhs_count = 2;
            
            // Production 2: E' -> + T E'
            strcpy(g->productions[1].lhs, "E'");
            strcpy(g->productions[1].rhs[0], "+");
            strcpy(g->productions[1].rhs[1], "T");
            strcpy(g->productions[1].rhs[2], "E'");
            g->productions[1].rhs_count = 3;
            
            // Production 3: E' -> ε
            strcpy(g->productions[2].lhs, "E'");
            strcpy(g->productions[2].rhs[0], "ε");
            g->productions[2].rhs_count = 1;
            
            // Production 4: T -> F T'
            strcpy(g->productions[3].lhs, "T");
            strcpy(g->productions[3].rhs[0], "F");
            strcpy(g->productions[3].rhs[1], "T'");
            g->productions[3].rhs_count = 2;
            
            // Production 5: T' -> * F T'
            strcpy(g->productions[4].lhs, "T'");
            strcpy(g->productions[4].rhs[0], "*");
            strcpy(g->productions[4].rhs[1], "F");
            strcpy(g->productions[4].rhs[2], "T'");
            g->productions[4].rhs_count = 3;
            
            // Production 6: T' -> ε
            strcpy(g->productions[5].lhs, "T'");
            strcpy(g->productions[5].rhs[0], "ε");
            g->productions[5].rhs_count = 1;
            
            // Production 7: F -> ( E )
            strcpy(g->productions[6].lhs, "F");
            strcpy(g->productions[6].rhs[0], "(");
            strcpy(g->productions[6].rhs[1], "E");
            strcpy(g->productions[6].rhs[2], ")");
            g->productions[6].rhs_count = 3;
            
            // Production 8: F -> id
            strcpy(g->productions[7].lhs, "F");
            strcpy(g->productions[7].rhs[0], "id");
            g->productions[7].rhs_count = 1;
            
            g->prod_count = 8;
            
            // Terminals
            strcpy(g->terminals[0], "id");
            strcpy(g->terminals[1], "+");
            strcpy(g->terminals[2], "*");
            strcpy(g->terminals[3], "(");
            strcpy(g->terminals[4], ")");
            strcpy(g->terminals[5], "$");
            g->terminal_count = 6;
            
            // Non-terminals
            strcpy(g->non_terminals[0], "E");
            strcpy(g->non_terminals[1], "E'");
            strcpy(g->non_terminals[2], "T");
            strcpy(g->non_terminals[3], "T'");
            strcpy(g->non_terminals[4], "F");
            g->non_terminal_count = 5;
            break;
        }
        
        default:
            printf("Invalid grammar choice\n");
            break;
    }
}
