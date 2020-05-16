int main(int argc, char** argv) {

    int x = 3;
    char y = 'a';

    while (x < y 
            || x == y 
         && argc > 0)
        y++;
    
    while (y
            > '0') {
        char z = '1';
        while (z > y
            || z == x) {
            z--;
            break;
        }
        while (x < 10)
            x += 
            2;
        z++, y--;
        continue;
    }

    return 0;
}