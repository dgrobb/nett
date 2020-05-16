int main(int argc, char** argv) {
    int x = 5;
    int y = 10;
    
    if (argc == 1) {
        return x + y;       
    } else if (argc == 2) {
        return 
                x * y;
    } else if (argc == 3) {
        return 
            x - y;
    } else if (argc == 4) {
        return x
             / y;
    } else {
        return (x + y + 5 /
             10
               * 5);
    }
    return x
        + y;
}