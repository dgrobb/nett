int main(int argc, char** argv) {

    int aVeryLongParameterNameHere = 2;
    int anotherLongParameterNameForLuck = 4;
    int howAboutAnotherOne = 5;

    int hey = aVeryLongParameterNameHere +
            howAboutAnotherOne;
    
    int there = anotherLongParameterNameForLuck +
        howAboutAnotherOne / aVeryLongParameterNameHere
          * argc;
    
    int param = there + aVeryLongParameterNameHere
                - howAboutAnotherOne;

    return 0;
}