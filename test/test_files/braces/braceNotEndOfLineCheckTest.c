struct Blah { int a;
};

union Baz { int x;
};

enum Fred {A, 
    B, C
};

int main(int argc, char** argv) { argc++;

    if (argc) {return 0;
    } else {return 1;
    }

    for (int i = 0; i < argc; i++) {argc--;
    }

    do {argc--;
    } while (argc > 0);

    while (argc > 0) {argc--;
    };

    switch (argc) { case 1:
        case 2: {return 5;
        }
    }

    return 0;
}