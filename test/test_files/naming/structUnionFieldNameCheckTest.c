// This is a struct
struct MyStruct {
    int correctCamelCase;
    float (*IncorrectCamelCase)(int, int);
    char someChar;
};

// This is a union
union MyUnion {
    char incorrect_lower_snake_case;
    float INCORRECT_UPPER_SNAKE_CASE;
    char* somePtr;
    int* anotherPointer;
};

int main(int argc, char** argv) {
    return 0;
}