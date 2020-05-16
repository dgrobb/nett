// Our validator
int some_really_poorly_named_validator_that_makes_life_hard(char* string) {
    return 0;
}

// Runs a checking function over a provided string
void validate_string(char* string, int* result, int (*checker)(char*)) {
    *result = checker(string);
}

int main(int argc, char** argv) {
    // This is going to be a comment that is really, really, really, really quite long. A bit too long unfortunately
    char* stringToCheck = "Here we have a string which is really just tooo long";
    char* otherToCheck = "Another string which is really testing these limits.";

    int theResult;
    validate_string(stringToCheck, &theResult, some_really_poorly_named_validator_that_makes_life_hard);

    return theResult;
}