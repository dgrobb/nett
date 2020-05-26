int main(int argc, char** argv) {
    int x = argc-- ? 2 : 4 ;
    int y = (x == 1)  ?  3  :    ++argc;
    int z = (x + 1)
            ?  3 : argc;
    int a = (x - 1) ?
            argc++  : 0;
    int b = x++ ? 2
            : 3;
    int c = a ?
            b :
            c;
    return 0;
}