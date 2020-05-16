#include <stdio.h>

int main(int argc, char** argv) {

    printf ("Too much whitespace before l-paren\n");
    
    printf(
            "Bad argument positioning for both parentheses\n"
            );

    printf
            ("Bad l-paren location\n");

    printf("Here are arguments with bad comma spacing: %d %d %d\n", 
            argc , argc + 1,  argc + 2);

    return 0;
}