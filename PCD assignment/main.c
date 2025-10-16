#include "compiler.h"

void print_header() {
    printf("\n");
    printf("╔═══════════════════════════════════════════════════════════════╗\n");
    printf("║          COMPILER DESIGN - LL & LR PARSER                     ║\n");
    printf("║          Built from Scratch in C Language                     ║\n");
    printf("╚═══════════════════════════════════════════════════════════════╝\n");
    printf("\n");
}

void print_menu() {
    printf("\n");
    printf("╔═══════════════════════════════════════════════════════════════╗\n");
    printf("║                         MAIN MENU                             ║\n");
    printf("╠═══════════════════════════════════════════════════════════════╣\n");
    printf("║  1. Load Sample Grammar                                       ║\n");
    printf("║  2. Display Grammar                                           ║\n");
    printf("║  3. Perform Lexical Analysis                                  ║\n");
    printf("║  4. Compute FIRST and FOLLOW Sets                             ║\n");
    printf("║  5. Build LL Parsing Table                                    ║\n");
    printf("║  6. Parse with LL Parser                                      ║\n");
    printf("║  7. Build LR Items                                            ║\n");
    printf("║  8. Parse with LR Parser                                      ║\n");
    printf("║  9. Run Complete Analysis (Lexer + LL + LR)                   ║\n");
    printf("║  0. Exit                                                      ║\n");
    printf("╚═══════════════════════════════════════════════════════════════╝\n");
    printf("\nEnter your choice: ");
}

void print_grammar_menu() {
    printf("\n");
    printf("╔═══════════════════════════════════════════════════════════════╗\n");
    printf("║                    SELECT GRAMMAR                             ║\n");
    printf("╠═══════════════════════════════════════════════════════════════╣\n");
    printf("║  1. Arithmetic Expression Grammar (Left Recursive)            ║\n");
    printf("║     E -> E + T | T                                            ║\n");
    printf("║     T -> T * F | F                                            ║\n");
    printf("║     F -> ( E ) | id                                           ║\n");
    printf("║                                                               ║\n");
    printf("║  2. Simple LL(1) Grammar                                      ║\n");
    printf("║     S -> A B                                                  ║\n");
    printf("║     A -> a | ε                                                ║\n");
    printf("║     B -> b                                                    ║\n");
    printf("║                                                               ║\n");
    printf("║  3. Expression Grammar (LL(1) Compatible)                     ║\n");
    printf("║     E -> T E'                                                 ║\n");
    printf("║     E' -> + T E' | ε                                          ║\n");
    printf("║     T -> F T'                                                 ║\n");
    printf("║     T' -> * F T' | ε                                          ║\n");
    printf("║     F -> ( E ) | id                                           ║\n");
    printf("╚═══════════════════════════════════════════════════════════════╝\n");
    printf("\nEnter your choice: ");
}

int main() {
    Grammar grammar;
    FirstFollowSet first_sets[MAX_SYMBOLS];
    FirstFollowSet follow_sets[MAX_SYMBOLS];
    bool grammar_loaded = false;
    bool first_follow_computed = false;
    bool ll_table_built = false;
    
    int choice;
    char input[1000];
    
    print_header();
    
    while (1) {
        print_menu();
        scanf("%d", &choice);
        getchar(); // Consume newline
        
        switch (choice) {
            case 1: {
                print_grammar_menu();
                int grammar_choice;
                scanf("%d", &grammar_choice);
                getchar();
                
                load_sample_grammar(&grammar, grammar_choice);
                grammar_loaded = true;
                first_follow_computed = false;
                ll_table_built = false;
                
                printf("\n✓ Grammar loaded successfully!\n");
                print_grammar(&grammar);
                break;
            }
            
            case 2: {
                if (!grammar_loaded) {
                    printf("\n✗ Please load a grammar first!\n");
                    break;
                }
                print_grammar(&grammar);
                break;
            }
            
            case 3: {
                printf("\nEnter input string to analyze: ");
                fgets(input, sizeof(input), stdin);
                input[strcspn(input, "\n")] = 0; // Remove newline
                
                init_lexer(input);
                print_tokens();
                break;
            }
            
            case 4: {
                if (!grammar_loaded) {
                    printf("\n✗ Please load a grammar first!\n");
                    break;
                }
                
                init_ll_parser(&grammar);
                compute_first_sets(&grammar, first_sets);
                compute_follow_sets(&grammar, first_sets, follow_sets);
                first_follow_computed = true;
                
                print_first_follow_sets(first_sets, grammar.non_terminal_count);
                
                printf("\n=== FOLLOW SETS ===\n");
                for (int i = 0; i < grammar.non_terminal_count; i++) {
                    if (follow_sets[i].follow_count > 0) {
                        printf("FOLLOW(%s) = { ", follow_sets[i].symbol);
                        for (int j = 0; j < follow_sets[i].follow_count; j++) {
                            printf("%s", follow_sets[i].follow[j]);
                            if (j < follow_sets[i].follow_count - 1) printf(", ");
                        }
                        printf(" }\n");
                    }
                }
                break;
            }
            
            case 5: {
                if (!grammar_loaded) {
                    printf("\n✗ Please load a grammar first!\n");
                    break;
                }
                
                if (!first_follow_computed) {
                    init_ll_parser(&grammar);
                    compute_first_sets(&grammar, first_sets);
                    compute_follow_sets(&grammar, first_sets, follow_sets);
                    first_follow_computed = true;
                }
                
                build_ll_table(&grammar, first_sets, follow_sets);
                ll_table_built = true;
                
                printf("\n=== LL PARSING TABLE ===\n");
                printf("%-15s", "Non-Terminal");
                for (int i = 0; i < grammar.terminal_count; i++) {
                    printf("%-15s", grammar.terminals[i]);
                }
                printf("\n");
                printf("------------------------------------------------------------------------------\n");
                
                // Print table (simplified display)
                printf("\n✓ LL Parsing table built successfully!\n");
                printf("Total entries: %d\n", get_ll_table_size());
                break;
            }
            
            case 6: {
                if (!grammar_loaded) {
                    printf("\n✗ Please load a grammar first!\n");
                    break;
                }
                
                if (!ll_table_built) {
                    init_ll_parser(&grammar);
                    compute_first_sets(&grammar, first_sets);
                    compute_follow_sets(&grammar, first_sets, follow_sets);
                    build_ll_table(&grammar, first_sets, follow_sets);
                    first_follow_computed = true;
                    ll_table_built = true;
                }
                
                printf("\nEnter input string to parse: ");
                fgets(input, sizeof(input), stdin);
                input[strcspn(input, "\n")] = 0;
                
                bool result = parse_ll(input);
                
                if (result) {
                    printf("\n✓ INPUT ACCEPTED BY LL PARSER\n");
                } else {
                    printf("\n✗ INPUT REJECTED BY LL PARSER\n");
                }
                break;
            }
            
            case 7: {
                if (!grammar_loaded) {
                    printf("\n✗ Please load a grammar first!\n");
                    break;
                }
                
                init_lr_parser(&grammar);
                build_lr_items(&grammar);
                build_lr_table(&grammar);
                break;
            }
            
            case 8: {
                if (!grammar_loaded) {
                    printf("\n✗ Please load a grammar first!\n");
                    break;
                }
                
                init_lr_parser(&grammar);
                
                printf("\nEnter input string to parse: ");
                fgets(input, sizeof(input), stdin);
                input[strcspn(input, "\n")] = 0;
                
                bool result = parse_lr(input);
                
                if (result) {
                    printf("\n✓ INPUT ACCEPTED BY LR PARSER\n");
                } else {
                    printf("\n✗ INPUT REJECTED BY LR PARSER\n");
                }
                break;
            }
            
            case 9: {
                if (!grammar_loaded) {
                    printf("\n✗ Please load a grammar first!\n");
                    break;
                }
                
                printf("\nEnter input string to analyze: ");
                fgets(input, sizeof(input), stdin);
                input[strcspn(input, "\n")] = 0;
                
                printf("\n");
                printf("╔═══════════════════════════════════════════════════════════════╗\n");
                printf("║              COMPLETE COMPILATION ANALYSIS                    ║\n");
                printf("╚═══════════════════════════════════════════════════════════════╝\n");
                
                // Lexical Analysis
                init_lexer(input);
                print_tokens();
                
                // LL Parsing
                if (!ll_table_built) {
                    init_ll_parser(&grammar);
                    compute_first_sets(&grammar, first_sets);
                    compute_follow_sets(&grammar, first_sets, follow_sets);
                    build_ll_table(&grammar, first_sets, follow_sets);
                    ll_table_built = true;
                }
                
                bool ll_result = parse_ll(input);
                
                // LR Parsing
                init_lr_parser(&grammar);
                bool lr_result = parse_lr(input);
                
                printf("\n");
                printf("╔═══════════════════════════════════════════════════════════════╗\n");
                printf("║                    ANALYSIS SUMMARY                           ║\n");
                printf("╠═══════════════════════════════════════════════════════════════╣\n");
                printf("║  Lexical Analysis:   COMPLETE                                 ║\n");
                printf("║  LL Parser:          %-40s ║\n", ll_result ? "ACCEPTED ✓" : "REJECTED ✗");
                printf("║  LR Parser:          %-40s ║\n", lr_result ? "ACCEPTED ✓" : "REJECTED ✗");
                printf("╚═══════════════════════════════════════════════════════════════╝\n");
                break;
            }
            
            case 0:
                printf("\n");
                printf("╔═══════════════════════════════════════════════════════════════╗\n");
                printf("║              Thank you for using the compiler!                ║\n");
                printf("╚═══════════════════════════════════════════════════════════════╝\n");
                printf("\n");
                return 0;
            
            default:
                printf("\n✗ Invalid choice! Please try again.\n");
                break;
        }
        
        printf("\nPress Enter to continue...");
        getchar();
    }
    
    return 0;
}
