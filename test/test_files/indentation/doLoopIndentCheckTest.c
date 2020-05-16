int main(int argc, char** argv) {

    int x = argc;
    
    do
        x++;
    while (x < 10);

    do {
        char y = x++;
        y += ++x;
        
        do 
            x++;
        while (y < '0');
    } while (x < 255);
    
    x = 0;

    do
          x++;
      while (x < 10);

     do {
         char y = x++;
               y += ++x;
     continue;
   } while (x < 255);

    return 0;
}