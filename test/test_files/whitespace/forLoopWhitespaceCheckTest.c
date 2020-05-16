int main(int argc, char** argv) {
    // For loops should look like: for (...
    for 
        (int i = 0; i < 10; i++) {
    }
    // Construct parentheses...
    for (
            int i = 0; i < 10; i++
            ) {
    }
    // Whitespace in parens...
    for (  int i = 0; i < 10; ++i   ) {
    }
    // Braces/parens whitespace
    for    (int i = 0; i < 10; i++)   {
    }
    // Semicolons whitespace
    for (;;) {
    }
    for ( ; ; ) {
    }
    for (int i = 0 ; i < 10 ; i++) {
    }    
    // Missing components semicolon whitespace
    for (; argc > 0; argc--) {
    }
    for (  ; argc > 0; argc--) {
    }
    for (
            ; argc > 0; argc--) {
    }

    for (int argc = 0;; argc--) {
    }
    for (int argc = 0; ; argc--) {
    }
    for (int argc = 0;
            ; argc--) {
    }

    for (int argc = 0; argc < 1;) {
    }
    for (int argc = 0; argc < 1;  ) {
    }
    for (int argc = 0; argc < 1;
            ) {
    }

    return 0;
}