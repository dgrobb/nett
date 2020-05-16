int main(int argc, char** argv) {

    int x = 0;

    if (argc == 1) {
        char y = 'a';
        x += 2;
        if (x--)
            return y;
        else if (x == 2) 
            return 3;
        else {
            x++;
        }
    } else {
        return 3;
    }

      if (argc == 3) {
          char y = 'b';
     x += 2;
         if (x--)
             return y;
            else if (x == 2) 
            return 3;
     else {
             x++, y--;
         }
    } else {
        return 3;
    }

    return 0;
}