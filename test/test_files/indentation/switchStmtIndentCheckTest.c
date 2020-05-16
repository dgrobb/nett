// Some comment...
int good_switch_example(int argc, int x) {
    
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
                    switch (x) {
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
            if (x > 2) {
                return 1;
            }
        default:
            x--;
    }

    switch (argc)
        case 10:
        case 11: {
            x++;
            switch (x)
                case 11:
                case 12:
                    x--;
            return x;
        }
}

int main(int argc, char** argv) {

    int x = 5;

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
                    switch (x) {
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
          if (x > 2) {
                return 1;
            }
         default:
            x--;
        }

  switch (argc)
         case 10:
        case 11: {
              x++;
            switch (x)
                case 11:
                 case 12:
                x--;
          return x;
        }

    return x;
}