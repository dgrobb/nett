#define NUMBER 3

int main(int argc, char** argv) {

    // Switch should look like switch (...
    switch
            (argc) {
        // Too much whitespace between case and value
        case  1:
            return 0;
    }

    // Too much space between switch and condition
    switch  (argc) {
        // Too much whitespace between value and colon
        case NUMBER :
            return 0;
    }

    // Too much condition whitespace
    switch ( argc ) {
        // Too much space between colon and opening brace
        case 1:  {
            return 0;
        }
    }

    // Bad condition positioning
    switch (
            argc
            ) {
        // Case and value not on the same line
        case 
                1:
            return 5;
    }

    // Too much space between condition and open brace
    switch (argc)  {
        // Case value and colon not on the same line
        case 1
                :
            return 5;
    }
    
    return 0;
}