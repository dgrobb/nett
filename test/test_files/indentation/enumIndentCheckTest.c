enum Foo;

enum Foo {
    A, B, C, D
};

enum Bar {
    E, F,
    G, H,
    I
};

typedef enum Baz {
    J = 5, K = 2, L = 'x'
} Baz;

   enum BadFoo;

 enum BadFoo {
     BA, BB, BC,
      BD, BE, BF
  };

 typedef enum BadBar {
    BBA, 
     BBB, BBC
} BadBar;

int main(int argc, char** argv) {

    enum Nested {
        NA, NB, NC    
    };

    typedef enum DefNested {
        ND, NE, 
        NF
    } DefNested;

     enum BadNested {
          NG, NH, NI    
   };

typedef enum BadDefNested {
        NJ, NK, 
         NL
     } BadDefNested;

    return 0;
}