// Some words
enum Words {
    A, B, C
};

typedef enum Words Words;

// A struct
typedef struct Blah {
    int a;
    int b
} Blah;

typedef int MyInt;

// Another struct
struct BadBlah {
    int x;
    int y;
};
typedef struct BadBlah badBlah;

typedef int bad_int;

typedef enum BadFoo {
    X, Y, Z
} Bad_Foo;

int main(int argc, char** argv) {
    return 0;
}