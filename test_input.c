#include <stdio.h>
#include <stdlib.h>

int main() {
    int a = 10;
    int b = 20;
    float result = 0.0;
    
    // Calculate sum
    result = a + b;
    printf("Sum: %f\n", result);
    
    /* Check if a is less than b */
    if (a < b) {
        printf("a is less than b\n");
    } else {
        printf("a is greater than or equal to b\n");
    }
    
    // Loop example
    for (int i = 0; i < 5; i++) {
        printf("Iteration: %d\n", i);
    }
    
    while (a != b) {
        a++;
    }
    
    char ch = 'X';
    char* str = "Hello, World!";
    
    return 0;
}
