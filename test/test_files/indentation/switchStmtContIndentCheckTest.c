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
    return 0;
}

// A function
void blah(int argc) {
    switch (argc) {
        case 2:
            switch (argc) {
                case 4:
                    if (argc == 1 || argc == 2 ||
                            argc == 3) {
                        return;
                    } else if (argc == 5 ||
                            argc == 6) {
                        return;
                    } else {
                        return;
                    }
            }
        case 3:
            switch (argc) {
                case 3:
                    for (int i = 0; i < argc;
                            i++) {
                        break;
                    }
            }
        case 4:
            switch (argc) {
                case 3:
                    do {
                        break;
                    } while (argc == 2 && argc == 3 &&
                            argc == 4);
            }
        case 5:
            switch (argc) {
                case 3:
                    while (argc == 4 &&
                            argc == 5) {
                        break;
                    }
            }
    }
}