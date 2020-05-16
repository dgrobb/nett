struct GoodName;
union badCamelCase;

// A struct please...
struct GoodName {
    int a;
};

// And a union...
union badCamelCase {
    char x;
};

// Another union please...
union bad_snake_case {
    float y;
};

// Finally, another struct
struct OTHER_SNAKE_CASE {
    double down;
};

int main(int argc, char** argv) {
    return 0;
}