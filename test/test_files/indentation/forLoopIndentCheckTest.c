int main(int argc, char** argv) {

    for (int i = 0; i < 10; i++) {
        i++;
        for (int j = 0; j < i; j++) {
            j += 2;
            for (int k = 0; k < j; k++) {
                k = k - 1;
                break;
            }
            continue;
        }
    }

      for (int i = 0; i < 10; i++) {
        for (int j = 0; j < i; j++) {
            for (int k = 0; k < j; k++) {
               k += 1;
                  break;
            }
               continue;
    }
        i++;
    }
    
    return 0;
}