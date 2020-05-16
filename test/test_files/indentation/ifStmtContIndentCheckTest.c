int main(int argc, char** argv) {

    int x = 0;

    if (argc == 1 ||
            argc == 2) {
        char y = 'a';
        x += 2;
        if (x + argc
                > 12 || x - argc
                < 10)
            return y;
        else if (x == 2) 
            x += 
                    2;
        else {
            x++;
        }
    } else {
        return 3;
    }

    if (argc == 3 ||
        argc == 4 &&
      argc 
          < 2) {
        char y = 'b';
        x += 2;
       
        if (x--)
            x +=
              2;
        else if (x == 2 ||
                 x == 3) 
            return 3;
        else {
            x++, y--;
        }
    } else 
        x -=
         1;
    
    return 0;
}