enum Blah 
{
    A, B, C
};

struct Fred
{
    int x;
    int y;
};

union Bob
{
    int a;
    char b;
};

int main(int argc, char** argv) {
    
    if (argc)
    {
        return 2;
    } else if (argc + 1) 
    {
        return 3;
    } else 
    {
        return 1;
    }

    if (argc) {
        return 3;
    }
    else {
        return 5;
    }

    for (int i = 0; i < 10; i++) 
    {
        i--;
    }

    while (argc > 0)
    {
        argc--;
    }   

    do 
    {
        argc--;
    } 
    while (argc > 0);

    switch (argc)
    {
        case 1:
        case 2: 
        {
            return 5;
        }
        default:
            break;
    }

    return 0;
}