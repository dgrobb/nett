// Some random struct
struct MyStruct {
    int a;
    int b;
};

// a comment here
int globalVarHere = 0;

/* another comment
   here */
char anotherGlobal = '1';

/** Some doxygen commenting */
float yetAnotherGlobal = 2.0;

/// More weird commenting
const char* aGlobalString = "three";

//! And the final type of comment
int finalGlobal = 4;

// Now, this comment is a bit off

int commentTooFar = 5;
int aCommentToTheSide = 6; // Here we go off to the side
int noCommentHere = 7;

// This is also commented
struct MyStruct something;

int main(int argc, char** argv) {
    int localVariable = 8;
    return localVariable;
}