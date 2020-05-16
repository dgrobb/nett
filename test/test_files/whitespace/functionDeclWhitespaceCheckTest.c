// Extra space at the semi-colon
void no_params_here(void) ;

// Extra space unspecified parameters
void unspec_params( );

// Extra space between name and parameters
void name_and_param_space (void);

// Params and name on different lines
void different_lines
        (void);

// Semi-colon on its own line
void one_parameter(int extraSpace)
        ;

// Extra space around no parameters
void no_params_here( void ) {
    return;
}

// Extra space around a single parameter
void one_parameter( int extraSpace ) {
    return;
}

// Extra space around multiple parameters
void many_parameters( int a, int b, int c ) {
    return;
}

// Extra space around commas separating parameters
void bad_commas(int a , int b,  char c) {
    return;
}

// Extra space -> newlines
void bad_newlines(
        int a , int b,  char c
        ) {
    return;
}

// How about function pointers
void function_pointers(void (*foo)( void )) {
    return;
}

// Some more function pointers
void more_function_pointers(void (*foo)( int a ,  char b )) {
    return;
}

int
main(int argc, char** argv) {
    return 0;
}