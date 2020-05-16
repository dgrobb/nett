int main(int argc, char** argv) {

    int someParameter = argc + 5;
    int x = 2;
    
    switch (argc) {
        case 1:
        case 2: {
            x++;
            return x;
        }
        case 4: 
            x += 5;
            switch (x) {
                case 10:
                    switch (someParameter 
                            - 
                          argc * 2) {
                        case 1: {
                            x--;
                            return 1;
                        }
                    }
                case 11:
                    x += 5;
                    return argc;
                default:
                    return 2;
            }
        default:
            x--;
    }

    switch (argc - 2 
            / someParameter)
        case 10:
        case 11: {
            x++;
            switch (someParameter + 2
                - argc)
                case 11:
                case 12:
                    x--;
            return x;
        }
}