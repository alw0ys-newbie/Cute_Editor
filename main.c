/*** includes ***/

#include "cute.h"

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

/*** init ***/

int main(void)
{
    editorInit();
    while (1) {
        screenRefrech();
        editorProcessKeyPressed();
    }
    return 0;
}