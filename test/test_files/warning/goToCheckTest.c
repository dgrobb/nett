int main(int argc, char** argv) {

    int i = 0;
LOOP:
    if (i < 10) {
        i++;
        goto LOOP;
    }
    return 0;
}