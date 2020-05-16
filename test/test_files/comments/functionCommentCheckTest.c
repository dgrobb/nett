#include "functionCommentCheckTest.h"

// This function only has a comment at the declaration
void declaration_comment_only(void);
int definition_comment_only(void);

void no_comment(void) {
    // this function has no comments at all
}

void declaration_comment_only(void) {
    // this function only has a comment at the declaration
}

// This function only has a comment at the definition
int definition_comment_only(void) {
    return 1;
}

int declaration_in_header_has_comment(void) {
    return 0;
}

int main(int argc, char** argv) {
    return 0;
}