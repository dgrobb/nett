// Space between typedef and struct/union
typedef  struct Foo {
    int a;
    char b ; // Extra space between field and semi-colon
} Foo ; // Extra space between name and semi-colon

// typedef and struct/union on different lines
typedef
        union Bar {
    float x
            ;   // Semi-colon on the wrong line
} Bar;

// Space between enum and name
struct  Bax {
    int x;
};

// Space between name and open brace
union Baz  {
    int y;
}
; // Semi-colon on the wrong line

int main(int argc, char** argv) {
    
    // Extra space between name and '{' (anonymous struct/union)
    struct  {
        char blah;
    } anon;

    return 0;
}