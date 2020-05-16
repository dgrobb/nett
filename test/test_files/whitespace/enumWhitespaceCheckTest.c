// Space between typedef and enum
typedef  enum Foo {
    A, B, C
} Foo ;

// typedef and enum on different lines
typedef
        enum Bar {
    D, E , F
} Bar;

// Space between enum and name
enum  Bax {
    G, H, I
};

// Space between name and open brace
enum Baz  {
    J, K, L
}
;

int main(int argc, char** argv) {
    
    // Extra space between name and '{' (anonymous enum)
    enum  {
        M,
        N
    } anon;

    return 0;
}