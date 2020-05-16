int main(int argc, char** argv) {

    int aVeryLongParameterNameHere = 2;
    int anotherLongParameterNameForLuck = 4;
    int howAboutAnotherOne = 5;
    
    int hey, there;

    hey = aVeryLongParameterNameHere +
            howAboutAnotherOne;
    
    there = anotherLongParameterNameForLuck +
        howAboutAnotherOne / aVeryLongParameterNameHere
          * argc;
    
    hey *= there + aVeryLongParameterNameHere
                * howAboutAnotherOne;

    return anotherLongParameterNameForLuck +
          howAboutAnotherOne;
}