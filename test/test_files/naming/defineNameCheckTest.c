#define incorrect_snake_case 'a'
#define CORRECT_SNAKE_CASE 'b'
#define IncorrectUpperCamelCase 'c'
#define incorrectLowerCamelCase 'd'
#undef incorrect_snake_case
#define incorrect_snake_case 'e'

int main(int argc, char** argv) {
    return 0;
}