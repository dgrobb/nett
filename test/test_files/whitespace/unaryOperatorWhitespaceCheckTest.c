int main(int argc, char** argv) {

    int x = 0;
    int y = 0;
    int* z;

    // Increment / Decrement operators
    x ++;
    x  --;
    -- x;
    ++  x;

    ++
            y;
    y
            --;

    // Logical negation operator
    x = ! x;
    x = !
            y;

    // Unary minus
    y = -  x;
    y = -
            x;

    // Address/dereference operators
    z = &   x;
    z = &
            x;

    x = * z;
    x = *
            z;

    return 0;
}