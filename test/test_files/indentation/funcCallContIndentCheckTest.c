// A function
int some_function(int a, int b) {
    return a + b;
}

int main(int argc, char** argv) {

    int aVeryLongParameterNameHere = 2;
    int anotherLongParameterNameForLuck = 4;
    int howAboutAnotherOne = 5;
    
    some_function(aVeryLongParameterNameHere, 
                anotherLongParameterNameForLuck + argc
             + howAboutAnotherOne);
    
    some_function(aVeryLongParameterNameHere,
            anotherLongParameterNameForLuck +
            howAboutAnotherOne);

    return some_function(anotherLongParameterNameForLuck,
        argc + howAboutAnotherOne);
}