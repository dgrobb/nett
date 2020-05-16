int main(int argc, char** argv) {

    if  (argc) {
        return 1;
    }

    if (argc)  {
        return 2;
    }

    if ( argc) {
        return 3;
    } else  {
        return 5;
    }

    if (argc ) {
        return 4;
    } else 
            if (argc + 1) {
        return 5;
    }

    if 
            (argc) {
        return 5;            
    }  else  if (
            argc + 1
            ) {
        return 2;
    }

    return 0;
}