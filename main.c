/*** includes ***/

#include "cute.h"

/*** init ***/

int main(void)
{
    editor_init();
    while (1) {
        screenRefrech();
        editorProcessKeyPressed();
    }
    return 0;
}