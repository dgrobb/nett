#include <stdbool.h>

// Some comment...
void no_continuation_indent_required(int a, int b) {
    return;
}

// What a nice function
int continuation_indent_no_multi_line(int someLongParamName,
        char anotherLongParamName, float andAThird) {
    return 0;
}

// Some convoluted continuation, but it is techincally correct...
void continuation_indent_multi_line(int
        thisOneWeSplitInABadWayButItIsCorrect, int anotherParamHere) {
    return;
}

// Function pointers are a pain because we can get this monstrosity...
void continuation_indent_function_pointer(int (*randomFunction)(int, 
        bool (*)(int, int), bool (*)(float, double, char), 
        char (*)(int, int)), int someParam) {
    return;
}

// Not enough of a continuation indent on this one
void bad_no_multi_line(int aLongParamNameThatJustKeepsGoing,
     char anotherParamIsNotIndentedEnough) {
    return;
}

// Too much, and too little continuation indentation here
void bad_multi_line(int 
            someParameter, bool anotherParameter, char
          anotherOne) {
    return;
}

// Our convoluted friend returns
void bad_function_pointerv(int (*randomFunction)(int, 
              bool (*)(int, int), bool (*)(float, double, char), 
             char (*)(int, int)), int someParam) {
    return;
}

int main(int argc, char** argv) {
    return 0;    
}
