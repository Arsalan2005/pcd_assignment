// Comprehensive lexical analyzer test
#include <stdio.h>

/* This program demonstrates
   all token types */

int main() {
    // Keywords and data types
    int a = 10;
    float b = 3.14;
    char c = 'x';
    double d = 2.718;
    
    // Operators
    int sum = a + 5;
    int diff = a - 3;
    int prod = a * 2;
    int quot = a / 2;
    int mod = a % 3;
    
    // Relational operators
    if (a > 5 && b < 10.0) {
        printf("Condition true\n");
    }
    
    // Logical operators
    if (a >= 10 || b <= 3.0) {
        printf("Another condition\n");
    }
    
    // Loop constructs
    for (int i = 0; i < 10; i++) {
        sum += i;
    }
    
    while (a != 0) {
        a--;
    }
    
    do {
        a++;
    } while (a < 5);
    
    // String literal
    char* str = "Hello, World!";
    
    // Switch statement
    switch (a) {
        case 1:
            printf("One\n");
            break;
        case 2:
            printf("Two\n");
            break;
        default:
            printf("Other\n");
    }
    
    return 0;
}
