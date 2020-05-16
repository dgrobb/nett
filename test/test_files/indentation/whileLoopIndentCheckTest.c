int main(int argc, char** argv) {

    int x = argc;
    char y = 'a';

    while (x)
        y++;
    
    while (y > '0') {
        char z = '1';
        while (z > y) {
            z--;
            break;
        }
        while (x < 10)
            x++;
        z++, y--;
        continue;
    }

    while (x)
      y++;
    
     while (y > '0') {
      char z = '1';
          while (z > y) {
            z--;
             break;
         }
        while (x < 10)
             x++;
         z++, y--;
      continue;
    }
}