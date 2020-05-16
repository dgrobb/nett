// A incorrect snake_case declaration
int incorrect_Snake_case_NAME(void);

// A correctly named function
void correct_function_name(char** a, int b[]) {
    // no body here
}

// Incorrect snake_case function
int incorrect_Snake_case_NAME(void) {
    return -1;
}

// Incorrect lowerCamelCase function
int (*incorrectCamelCaseLower(void))(void) {
    return incorrect_Snake_case_NAME;
}

// Incorrect CamelCaseUpper function
void IncorrectCamelCaseUpper(void) {
    // no body here
}

int main(int argc, char** argv) {
    return 0;
}