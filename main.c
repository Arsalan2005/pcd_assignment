#include "lexer.h"
#define _POSIX_C_SOURCE 200809L

int main(int argc, char* argv[]) {
    char* source = NULL;
    FILE* file = NULL;
    
    // Check if input file is provided
    if (argc > 1) {
        file = fopen(argv[1], "r");
        if (file == NULL) {
            fprintf(stderr, "Error: Could not open file '%s'\n", argv[1]);
            return 1;
        }
        
        // Get file size
        fseek(file, 0, SEEK_END);
        long fileSize = ftell(file);
        fseek(file, 0, SEEK_SET);
        
        // Allocate memory and read file
        source = (char*)malloc(fileSize + 1);
        if (source == NULL) {
            fprintf(stderr, "Error: Memory allocation failed\n");
            fclose(file);
            return 1;
        }
        
        fread(source, 1, fileSize, file);
        source[fileSize] = '\0';
        fclose(file);
    } else {
        // Use default sample code
        const char* defaultCode = 
            "#include <stdio.h>\n"
            "\n"
            "int main() {\n"
            "    int x = 10;\n"
            "    float y = 20.5;\n"
            "    char c = 'A';\n"
            "    \n"
            "    // This is a comment\n"
            "    if (x > 5) {\n"
            "        printf(\"x is greater than 5\\n\");\n"
            "    }\n"
            "    \n"
            "    /* Multi-line\n"
            "       comment */\n"
            "    for (int i = 0; i < x; i++) {\n"
            "        y += 1.5;\n"
            "    }\n"
            "    \n"
            "    return 0;\n"
            "}\n";
        source = (char*)malloc(strlen(defaultCode) + 1);
        if (source == NULL) {
            fprintf(stderr, "Error: Memory allocation failed\n");
            return 1;
        }
        strcpy(source, defaultCode);
    }
    
    printf("=================================================\n");
    printf("           LEXICAL ANALYZER OUTPUT\n");
    printf("=================================================\n");
    printf("%-20s %-20s %s\n", "TOKEN TYPE", "LEXEME", "POSITION");
    printf("-------------------------------------------------\n");
    
    // Initialize lexer
    initLexer(source);
    
    // Get and print all tokens
    Token token;
    do {
        token = getNextToken();
        if (token.type != TOKEN_EOF) {
            printToken(token);
        }
    } while (token.type != TOKEN_EOF);
    
    printf("=================================================\n");
    printf("Lexical analysis completed successfully!\n");
    printf("=================================================\n");
    
    free(source);
    return 0;
}
