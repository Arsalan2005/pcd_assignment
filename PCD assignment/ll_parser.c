#include "compiler.h"

static Grammar* grammar = NULL;
static FirstFollowSet first_sets[MAX_SYMBOLS];
static FirstFollowSet follow_sets[MAX_SYMBOLS];
static LLTableEntry ll_table[MAX_SYMBOLS * MAX_SYMBOLS];
static int ll_table_size = 0;

void init_ll_parser(Grammar* g) {
    grammar = g;
    ll_table_size = 0;
    memset(first_sets, 0, sizeof(first_sets));
    memset(follow_sets, 0, sizeof(follow_sets));
}

static inline bool has_epsilon(const char* symbol, const FirstFollowSet first_sets[], int count) {
    for (int i = 0; i < count; i++) {
        if (strcmp(first_sets[i].symbol, symbol) == 0) {
            for (int j = 0; j < first_sets[i].first_count; j++) {
                if (strcmp(first_sets[i].first[j], "ε") == 0) {
                    return true;
                }
            }
            return false; // Early exit if symbol found but no epsilon
        }
    }
    return false;
}

static inline void add_to_first(FirstFollowSet* set, const char* symbol) {
    // Check if already exists
    for (int i = 0; i < set->first_count; i++) {
        if (strcmp(set->first[i], symbol) == 0) return;
    }
    // Add if space available
    if (set->first_count < MAX_SYMBOLS) {
        strcpy(set->first[set->first_count++], symbol);
    }
}

static inline void add_to_follow(FirstFollowSet* set, const char* symbol) {
    // Don't add epsilon to follow sets
    if (strcmp(symbol, "ε") == 0) return;
    
    // Check if already exists
    for (int i = 0; i < set->follow_count; i++) {
        if (strcmp(set->follow[i], symbol) == 0) return;
    }
    // Add if space available
    if (set->follow_count < MAX_SYMBOLS) {
        strcpy(set->follow[set->follow_count++], symbol);
    }
}

void compute_first_sets(Grammar* g, FirstFollowSet sets[]) {
    // Initialize first sets
    for (int i = 0; i < g->non_terminal_count; i++) {
        strcpy(sets[i].symbol, g->non_terminals[i]);
        sets[i].first_count = 0;
    }
    
    bool changed = true;
    while (changed) {
        changed = false;
        
        for (int i = 0; i < g->prod_count; i++) {
            Production* prod = &g->productions[i];
            
            // Find the set for this LHS
            FirstFollowSet* lhs_set = NULL;
            for (int j = 0; j < g->non_terminal_count; j++) {
                if (strcmp(sets[j].symbol, prod->lhs) == 0) {
                    lhs_set = &sets[j];
                    break;
                }
            }
            
            if (lhs_set == NULL) continue;
            
            for (int r = 0; r < prod->rhs_count; r++) {
                const char* rhs = prod->rhs[r];
                int old_count = lhs_set->first_count;
                
                if (strcmp(rhs, "ε") == 0) {
                    add_to_first(lhs_set, "ε");
                } else if (is_terminal(rhs, g)) {
                    add_to_first(lhs_set, rhs);
                } else {
                    // Non-terminal
                    for (int j = 0; j < g->non_terminal_count; j++) {
                        if (strcmp(sets[j].symbol, rhs) == 0) {
                            for (int k = 0; k < sets[j].first_count; k++) {
                                add_to_first(lhs_set, sets[j].first[k]);
                            }
                            break;
                        }
                    }
                }
                
                if (lhs_set->first_count != old_count) {
                    changed = true;
                }
            }
        }
    }
}

void compute_follow_sets(Grammar* g, FirstFollowSet first_sets[], FirstFollowSet follow_sets[]) {
    // Initialize follow sets
    for (int i = 0; i < g->non_terminal_count; i++) {
        strcpy(follow_sets[i].symbol, g->non_terminals[i]);
        follow_sets[i].follow_count = 0;
    }
    
    // Add $ to start symbol's follow set
    for (int i = 0; i < g->non_terminal_count; i++) {
        if (strcmp(follow_sets[i].symbol, g->start_symbol) == 0) {
            add_to_follow(&follow_sets[i], "$");
            break;
        }
    }
    
    bool changed = true;
    while (changed) {
        changed = false;
        
        for (int i = 0; i < g->prod_count; i++) {
            Production* prod = &g->productions[i];
            
            for (int r = 0; r < prod->rhs_count; r++) {
                const char* symbol = prod->rhs[r];
                
                if (!is_non_terminal(symbol, g)) continue;
                
                // Find follow set for this symbol
                FirstFollowSet* symbol_follow = NULL;
                for (int j = 0; j < g->non_terminal_count; j++) {
                    if (strcmp(follow_sets[j].symbol, symbol) == 0) {
                        symbol_follow = &follow_sets[j];
                        break;
                    }
                }
                
                if (symbol_follow == NULL) continue;
                
                int old_count = symbol_follow->follow_count;
                
                // If this is the last symbol or all following symbols can derive epsilon
                bool add_lhs_follow = true;
                
                // Add FIRST of next symbols (excluding epsilon) to FOLLOW of current symbol
                for (int k = r + 1; k < prod->rhs_count; k++) {
                    const char* next = prod->rhs[k];
                    
                    if (is_terminal(next, g)) {
                        add_to_follow(symbol_follow, next);
                        add_lhs_follow = false;
                        break;
                    } else {
                        // Add FIRST(next) - {epsilon}
                        for (int f = 0; f < g->non_terminal_count; f++) {
                            if (strcmp(first_sets[f].symbol, next) == 0) {
                                for (int m = 0; m < first_sets[f].first_count; m++) {
                                    if (strcmp(first_sets[f].first[m], "ε") != 0) {
                                        add_to_follow(symbol_follow, first_sets[f].first[m]);
                                    }
                                }
                                
                                if (!has_epsilon(next, first_sets, g->non_terminal_count)) {
                                    add_lhs_follow = false;
                                }
                                break;
                            }
                        }
                    }
                }
                
                // Add FOLLOW(LHS) to FOLLOW(symbol)
                if (add_lhs_follow) {
                    for (int j = 0; j < g->non_terminal_count; j++) {
                        if (strcmp(follow_sets[j].symbol, prod->lhs) == 0) {
                            for (int k = 0; k < follow_sets[j].follow_count; k++) {
                                add_to_follow(symbol_follow, follow_sets[j].follow[k]);
                            }
                            break;
                        }
                    }
                }
                
                if (symbol_follow->follow_count != old_count) {
                    changed = true;
                }
            }
        }
    }
}

void build_ll_table(Grammar* g, FirstFollowSet first_sets[], FirstFollowSet follow_sets[]) {
    ll_table_size = 0;
    
    for (int i = 0; i < g->prod_count; i++) {
        Production* prod = &g->productions[i];
        
        for (int r = 0; r < prod->rhs_count; r++) {
            const char* first_symbol = prod->rhs[r];
            
            if (strcmp(first_symbol, "ε") == 0) {
                // Add to table for all terminals in FOLLOW(LHS)
                for (int j = 0; j < g->non_terminal_count; j++) {
                    if (strcmp(follow_sets[j].symbol, prod->lhs) == 0) {
                        for (int k = 0; k < follow_sets[j].follow_count; k++) {
                            if (ll_table_size < MAX_SYMBOLS * MAX_SYMBOLS) {
                                strcpy(ll_table[ll_table_size].non_terminal, prod->lhs);
                                strcpy(ll_table[ll_table_size].terminal, follow_sets[j].follow[k]);
                                ll_table[ll_table_size].production_index = i;
                                ll_table_size++;
                            }
                        }
                        break;
                    }
                }
            } else if (is_terminal(first_symbol, g)) {
                if (ll_table_size < MAX_SYMBOLS * MAX_SYMBOLS) {
                    strcpy(ll_table[ll_table_size].non_terminal, prod->lhs);
                    strcpy(ll_table[ll_table_size].terminal, first_symbol);
                    ll_table[ll_table_size].production_index = i;
                    ll_table_size++;
                }
            } else {
                // Non-terminal: add for all terminals in FIRST(first_symbol)
                for (int j = 0; j < g->non_terminal_count; j++) {
                    if (strcmp(first_sets[j].symbol, first_symbol) == 0) {
                        for (int k = 0; k < first_sets[j].first_count; k++) {
                            if (strcmp(first_sets[j].first[k], "ε") != 0) {
                                if (ll_table_size < MAX_SYMBOLS * MAX_SYMBOLS) {
                                    strcpy(ll_table[ll_table_size].non_terminal, prod->lhs);
                                    strcpy(ll_table[ll_table_size].terminal, first_sets[j].first[k]);
                                    ll_table[ll_table_size].production_index = i;
                                    ll_table_size++;
                                }
                            }
                        }
                        break;
                    }
                }
            }
        }
    }
}

int find_production(const char* non_terminal, const char* terminal) {
    for (int i = 0; i < ll_table_size; i++) {
        if (strcmp(ll_table[i].non_terminal, non_terminal) == 0 &&
            strcmp(ll_table[i].terminal, terminal) == 0) {
            return ll_table[i].production_index;
        }
    }
    return -1;
}

bool parse_ll(const char* input) {
    init_lexer(input);
    
    char stack[MAX_STACK][MAX_TOKEN_LEN];
    int stack_top = 0;
    
    // Push $ and start symbol
    strcpy(stack[stack_top++], "$");
    strcpy(stack[stack_top++], grammar->start_symbol);
    
    Token current_token = get_next_token();
    
    printf("\n=== LL PARSING ===\n");
    printf("%-30s %-30s %-30s\n", "STACK", "INPUT", "ACTION");
    printf("--------------------------------------------------------------------------------------\n");
    
    while (stack_top > 0) {
        // Print current state
        printf("%-30s", "");
        for (int i = stack_top - 1; i >= 0; i--) {
            printf("%s ", stack[i]);
        }
        printf("\t%-30s", current_token.lexeme);
        
        char* top = stack[stack_top - 1];
        
        if (strcmp(top, "$") == 0) {
            if (current_token.type == TOKEN_EOF) {
                printf("ACCEPT\n");
                return true;
            } else {
                printf("ERROR: Unexpected input\n");
                return false;
            }
        }
        
        if (is_terminal(top, grammar) || strcmp(top, "$") == 0) {
            if (strcmp(top, current_token.lexeme) == 0) {
                printf("MATCH %s\n", top);
                stack_top--;
                current_token = get_next_token();
            } else {
                printf("ERROR: Expected %s but got %s\n", top, current_token.lexeme);
                return false;
            }
        } else {
            // Non-terminal
            int prod_index = find_production(top, current_token.lexeme);
            
            if (prod_index == -1) {
                printf("ERROR: No production for [%s, %s]\n", top, current_token.lexeme);
                return false;
            }
            
            Production* prod = &grammar->productions[prod_index];
            printf("Use %s ->", prod->lhs);
            for (int i = 0; i < prod->rhs_count; i++) {
                printf(" %s", prod->rhs[i]);
            }
            printf("\n");
            
            stack_top--;
            
            // Push RHS in reverse order (skip epsilon)
            if (strcmp(prod->rhs[0], "ε") != 0) {
                for (int i = prod->rhs_count - 1; i >= 0; i--) {
                    strcpy(stack[stack_top++], prod->rhs[i]);
                }
            }
        }
    }
    
    printf("ERROR: Stack empty but input remaining\n");
    return false;
}

void print_first_follow_sets(FirstFollowSet sets[], int count) {
    printf("\n=== FIRST SETS ===\n");
    for (int i = 0; i < count; i++) {
        if (sets[i].first_count > 0) {
            printf("FIRST(%s) = { ", sets[i].symbol);
            for (int j = 0; j < sets[i].first_count; j++) {
                printf("%s", sets[i].first[j]);
                if (j < sets[i].first_count - 1) printf(", ");
            }
            printf(" }\n");
        }
    }
}

int get_ll_table_size() {
    return ll_table_size;
}
