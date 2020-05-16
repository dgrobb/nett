#ifndef __DEFN_IN_HEADER_CHECK_TEST__
#define __DEFN_IN_HEADER_CHECK_TEST__

// This function took a wrong path somewhere
int a_misplaced_definition(void) {
    return 1;
}

// Here we have a nice declaration as well
void some_declaration(int a, int b);

#endif