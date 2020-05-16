int main(int argc, char** argv) {

    for (int i = 0; 
            i < 10; 
            i++) {
        for (int j = 0; 
            j < i; 
                    j++) {
            for (int 
              k = 0; k < j; 
                      k++) {
                k += 1;
                break;
            }
            continue;
        }
        i++;
    }
    
    for (int i = 0; i < 10; i++)
        i += 
         argc;
         
    return 0;
}