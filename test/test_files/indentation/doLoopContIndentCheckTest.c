int main(int argc, char** argv) {

    int x = argc;
    int z = 5;

    do
        x++;
    while (x < 10 
            || x > 0);
    
    do {
        z--;
    } while (z > 
          argc);

    do {
        char y = x++;
        y += ++x;
        
        do 
            x +=
                2;
        while (y < '0' ||
            x++);
    } while (x
            < 255);

    return 0;
}