/*** includes ***/

#include "cute.h"

/*** init ***/

int main(void)
{
    editor_init();
    while (1) {
        screen_refresh();
        process_key_pressed();
    }
    return 0;
}