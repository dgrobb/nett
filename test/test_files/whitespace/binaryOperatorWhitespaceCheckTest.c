int main(int argc, char** argv) {

    int x = 5;
    int y = 6;
    int z;

    // Multiplicative operators
    z = x  *   y;
    z = x   /   y;
    z = x    %  y;

    // Additive operators
    z = x  +  y;
    z = x   -  y;

    // Bit-shift operators
    z = x  <<  y;
    z = y  >>  x;

    // Relational operators / comma operator
    if (y  <  x) {
        return x  >  y;
    } else if (y   >=   x ,  x  <=  y) {
        return argc;
    } 

    // Equality / Logical AND/OR / Bitwise AND/OR/XOR
    if (x  ==  y  ||  y  !=  x  &&  argc) {
        return x  &  y  |  z  ^  argc;
    }

    // Assignment operators
    z  =  x + y;
    z  *=  y;
    z   /=  x;
    x    %=     y;
    y   +=  y;
    z  -=   x;
    x   <<=  argc;
    y   >>=  argc;
    z    &=  x;
    x   ^=  x;
    y   |=   argc;

    return 0;
}