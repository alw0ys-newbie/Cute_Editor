
#include "cute.h"
#include <stdio.h>

int main(int argc, char** argv)
{
    editor_init();

    if (argc == 1) {
        while (1) {
            screen_refresh();
            process_key_pressed();
        }
    } else {
        printf("%s\n", argv[1]);
    }
    return 0;
}