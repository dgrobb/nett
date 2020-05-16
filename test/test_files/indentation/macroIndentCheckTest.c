#define MACRO(x, y) ((x) + (y))

int main(int argc, char** argv) {

    // Correct macro indentation
    for (int i = 0; i < 10; i++)
        MACRO(i, i);
    
    for (int i = 0; i < 10; i++)
        MACRO(i, 
                i);
    
    while (1)
        MACRO(1, 2);
    
    while (1)
        MACRO(1, 
                2);
    
    do
        MACRO(argc, argc);
    while (1);

    do
        MACRO(argc, 
                argc);
    while (1);

    if (argc)
        MACRO(1, 2);
    else
        MACRO(3, 4);

    if (argc)
        MACRO(1, 
                2);
    else
        MACRO(3, 
                4);

    MACRO(1, 2);
    MACRO(3, 
            4);

    return 0;
}

// Some comment
void incorrect_macro_indentation(int argc) {
    
    for (int i = 0; i < 10; i++)
          MACRO(i, i);
    
    for (int i = 0; i < 10; i++)
        MACRO(i, 
              i);
    
    while (1)
      MACRO(1, 2);
    
    while (1)
        MACRO(1, 
             2
            );
    
    do
           MACRO(argc, argc);
    while (1);

    do
        MACRO(argc, 
                     argc);
    while (1);

    if (argc)
         MACRO(1, 2);
    else
     MACRO(3, 4);

    if (argc)
        MACRO(1, 
                   2);
    else
        MACRO(3, 
                     4);

      MACRO(1, 2);
    MACRO(3, 
        4);
}