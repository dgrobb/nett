struct Foo;

struct Foo {
    int a;
    int b;

    enum {
        VAL7, VAL8 = 3, VAL9,
        VAL10, VAL11, VAL12 = 2
    } foovals;

    struct Bar {
        char x;
        char y;
    } bar;
};

typedef union Baz {
    int x;
    struct Foo* foo;

    struct {
        enum {
            E,
            F,
            G,
            H,
        } somevals;
        
        int x;
        int y;
    } bob;

    union Alice {
        int x;
        char y;
    } alice;
} Baz;

  struct BadFoo;

 struct BadFoo {
    int a;
      int b;

     enum {
          VAL1, VAL2 = 3, VAL3,
        VAL4, VAL5, VAL6 = 2
    } foovals;

     struct BadBar {
         char x;
        char 
            y;
} bar;
    };

    union BadBaz {
    int x;
    struct BadFoo* foo;

    struct {
        int x;
        int y;

       enum {
              A,
            B,
            C,
            D,
         } somevals;
    } bob;

    union BadAlice {
        int x;
          char y;
     } alice;
 };

int main(int argc, char** argv) {  

    struct Bob;
    struct Bob {
        int a;
        int b;
    };  

     struct Fred;
      struct Fred {
        int a;
         int b;
        };          
    return 0;
}