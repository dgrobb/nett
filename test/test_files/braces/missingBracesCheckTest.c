int main(int argc, char** argv) {

    if (argc) 
        return 5;
    else if (argc + 1)
        return 6;
    else
        return 7;
    
    while (argc) 
        argc--;

    for (int i = 0; i < 10; i++) 
        i++;
    
    do
        argc--;
    while (argc);

    switch (argc)
        case 1:
        case 2:
            return 5;

    return 0;
}