int main(int argc, char** argv) {

    // whitespace betwwen 'do' and '{'
    do  {
    } while (argc);

    // Whitespace between '}' and 'while'
    do {

    }   while (argc);

    // Whitespace betwwen 'while' and condition
    do {

    } while   (argc);
    
    // Condition on wrong line
    do {

    } while
            (argc);

    // Bad braces
    do {

    } while (
            argc
            );
    
    // Bad semicolon
    do {

    } while (argc)  ;
    do {

    } while (argc)
    ;

    // Whitespace within condition
    do {

    } while ( argc );

    return 0;
}