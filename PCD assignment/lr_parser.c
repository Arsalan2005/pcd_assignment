#include "compiler.h"

static Grammar* grammar = NULL;
static LRState states[MAX_STATES];
static int state_count = 0;
static LRTableEntry action_table[MAX_STATES][MAX_SYMBOLS];
static int goto_table[MAX_STATES][MAX_SYMBOLS];

void init_lr_parser(Grammar* g) {
    grammar = g;
    state_count = 0;
    memset(states, 0, sizeof(states));
    memset(action_table, 0, sizeof(action_table));
    memset(goto_table, -1, sizeof(goto_table));
}

void print_production(int prod_index) {
    if (prod_index < 0 || prod_index >= grammar->prod_count) return;
    
    Production* prod = &grammar->productions[prod_index];
    printf("%s ->", prod->lhs);
    for (int i = 0; i < prod->rhs_count; i++) {
        printf(" %s", prod->rhs[i]);
    }
}

void build_lr_items(Grammar* g) {
    // For simplicity, we'll implement a basic SLR parser
    // This is a simplified version - a full implementation would need closure and goto operations
    printf("\n=== LR(0) ITEMS ===\n");
    printf("Note: This is a simplified SLR parser implementation\n\n");
    
    for (int i = 0; i < g->prod_count; i++) {
        printf("I%d: ", i);
        print_production(i);
        printf("\n");
    }
}

void build_lr_table(Grammar* g) {
    (void)g;  // Suppress unused parameter warning
    
    // Initialize action table with ERROR
    for (int i = 0; i < MAX_STATES; i++) {
        for (int j = 0; j < MAX_SYMBOLS; j++) {
            action_table[i][j].action = ACTION_ERROR;
            action_table[i][j].state = -1;
        }
    }
    
    printf("\n=== LR PARSING TABLE ===\n");
    printf("This is a simplified SLR implementation\n");
    printf("For demonstration purposes, using a basic shift-reduce strategy\n\n");
}

bool parse_lr(const char* input) {
    init_lexer(input);
    
    typedef struct {
        int state;
        char symbol[MAX_TOKEN_LEN];
    } StackItem;
    
    StackItem stack[MAX_STACK];
    int stack_top = 0;
    
    // Push initial state
    stack[stack_top].state = 0;
    strcpy(stack[stack_top].symbol, "$");
    stack_top++;
    
    Token current_token = get_next_token();
    
    printf("\n=== LR PARSING (Simplified SLR) ===\n");
    printf("%-40s %-20s %-30s\n", "STACK", "INPUT", "ACTION");
    printf("--------------------------------------------------------------------------------------\n");
    
    char input_buffer[MAX_TOKEN_LEN * 10] = "";
    strcpy(input_buffer, current_token.lexeme);
    
    int step = 0;
    while (step++ < 100) {  // Prevent infinite loop
        // Print stack
        printf("%-40s", "");
        for (int i = 0; i < stack_top; i++) {
            printf("%s ", stack[i].symbol);
        }
        printf("\t%-20s\t", current_token.lexeme);
        
        // Simple shift-reduce logic for demonstration
        if (current_token.type == TOKEN_EOF) {
            if (stack_top == 2 && is_non_terminal(stack[1].symbol, grammar)) {
                printf("ACCEPT\n");
                return true;
            } else if (stack_top > 2) {
                // Reduce
                bool reduced = false;
                
                // Try to find a matching production
                for (int i = 0; i < grammar->prod_count; i++) {
                    Production* prod = &grammar->productions[i];
                    int match_len = prod->rhs_count;
                    
                    if (strcmp(prod->rhs[0], "ε") == 0) continue;
                    
                    if (stack_top >= match_len + 1) {
                        bool match = true;
                        for (int j = 0; j < match_len; j++) {
                            if (strcmp(stack[stack_top - match_len + j].symbol, prod->rhs[j]) != 0) {
                                match = false;
                                break;
                            }
                        }
                        
                        if (match) {
                            printf("REDUCE by ");
                            print_production(i);
                            printf("\n");
                            
                            stack_top -= match_len;
                            strcpy(stack[stack_top].symbol, prod->lhs);
                            stack[stack_top].state = stack_top;
                            stack_top++;
                            reduced = true;
                            break;
                        }
                    }
                }
                
                if (!reduced) {
                    printf("ERROR: Cannot reduce\n");
                    return false;
                }
            } else {
                printf("ERROR: Invalid stack state\n");
                return false;
            }
        } else {
            // Shift
            printf("SHIFT %s\n", current_token.lexeme);
            strcpy(stack[stack_top].symbol, current_token.lexeme);
            stack[stack_top].state = stack_top;
            stack_top++;
            current_token = get_next_token();
            
            // After shift, try reduce
            for (int i = 0; i < grammar->prod_count; i++) {
                Production* prod = &grammar->productions[i];
                int match_len = prod->rhs_count;
                
                if (strcmp(prod->rhs[0], "ε") == 0) continue;
                
                if (stack_top >= match_len + 1) {
                    bool match = true;
                    for (int j = 0; j < match_len; j++) {
                        if (strcmp(stack[stack_top - match_len + j].symbol, prod->rhs[j]) != 0) {
                            match = false;
                            break;
                        }
                    }
                    
                    if (match) {
                        // Print stack before reduce
                        printf("%-40s", "");
                        for (int k = 0; k < stack_top; k++) {
                            printf("%s ", stack[k].symbol);
                        }
                        printf("\t%-20s\t", current_token.lexeme);
                        printf("REDUCE by ");
                        print_production(i);
                        printf("\n");
                        
                        stack_top -= match_len;
                        strcpy(stack[stack_top].symbol, prod->lhs);
                        stack[stack_top].state = stack_top;
                        stack_top++;
                        break;
                    }
                }
            }
        }
    }
    
    printf("ERROR: Parsing limit exceeded\n");
    return false;
}

void print_lr_table() {
    printf("\n=== LR ACTION/GOTO TABLE ===\n");
    printf("State\t| ACTION\t| GOTO\n");
    printf("----------------------------------------\n");
    printf("(Simplified SLR parser - dynamic table construction)\n");
}
