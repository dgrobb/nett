struct Blah {
    int apple;
    char pear;
};

// A function...
void some_function(struct Blah* blah) {
    blah->apple = 0;    // correct
    blah-> pear = '2';  // incorrect
    blah ->apple = 3;   // incorrect
    blah
            ->apple = 4;    // incorrect
}

int main(int argc, char** argv) {
    
    struct Blah b;
    
    b.apple = 0;    // correct
    b. pear = 'a';  // incorrect
    b .apple = 1;   // incorrect
    b .
            apple = 1;  // incorrect
    
    return 0;
}