/*** includes ***/

#include "cute.h"
#include "data.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

/*** Macros ***/

#define CTRL_KEY(k) ((k) & 0x1f)
#define VERSION "0.0.1"
#define ERASE_LINE_AFTER_CURSOR "\x1b[K"
#define ERASE_SCREEN "\x1b[2J"
#define HIDE_CURSOR "\x1b[?25l"
#define SHOW_CURSOR "\x1b[?25h"
#define MOVE_CURSOR(x, y) "\x1b[" #x ";" #y "f"

/*** Global Variables */

editorConfig EC;

/*** Terminal ***/

void die(const char* s)
{
    write(STDOUT_FILENO, ERASE_SCREEN, 4);
    write(STDOUT_FILENO, MOVE_CURSOR(1, 1), 6);
    perror(s);
    exit(1);
}

void disableRawMode()
{
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &EC.orig_termios) == -1) {
        die("tcsetattr");
    };
}

void enableRawMode()
{

    struct termios raw;
    if (tcgetattr(STDIN_FILENO, &EC.orig_termios) == -1) {
        die("tecgetattr");
    };
    atexit(disableRawMode);

    raw = EC.orig_termios;

    raw.c_iflag &= ~(IXON | ICRNL | BRKINT | INPCK | ISTRIP);
    raw.c_oflag &= ~(OPOST);
    raw.c_cflag |= (CS8);
    raw.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) {
        die("tcsetattr");
    };
}

void getWindowSize(int* rows, int* cols)
{
    struct winsize ws;

    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
        die("getWindowSize");
    } else {
        *cols = ws.ws_col;
        *rows = ws.ws_row;
    }
}

void editorInit()
{
    enableRawMode();
    getWindowSize(&EC.rows, &EC.cols);
}

/*** Input ***/

char editorReadKey()
{
    int nread;
    char c;
    while ((nread = read(STDIN_FILENO, &c, 1)) != 1) {
        if (nread == -1 && errno != EAGAIN) {
            die("read");
        }
    }
    return c;
}

void editorProcessKeyPressed()
{
    char c;
    c = editorReadKey();
    switch (c) {
    case CTRL_KEY('q'):
        write(STDOUT_FILENO, ERASE_SCREEN, 4);
        write(STDOUT_FILENO, MOVE_CURSOR(1, 1), 6);
        exit(0);
        break;
    }
}

/*** output ***/

void appendRows(textBuffer_t screenRows)
{
    for (int i = 0; i < EC.rows; i++) {
        if (i == EC.rows / 3) {
            char welcome[80];
            int welcomeLen = snprintf(welcome, sizeof(welcome), "C.U.T.E -- version %s", VERSION);
            if (welcomeLen > EC.cols)
                welcomeLen = EC.cols;
            appendBuffer(screenRows, welcome);
        } else {
            appendBuffer(screenRows, "~");
        }
        appendBuffer(screenRows, ERASE_LINE_AFTER_CURSOR);
        if (i < EC.rows - 1) {
            appendBuffer(screenRows, "\r\n");
        }
    }
}

void screenRefrech()
{
    textBuffer_t newScreen = initBuffer();
    appendBuffer(newScreen, HIDE_CURSOR);
    appendBuffer(newScreen, MOVE_CURSOR(1, 1));
    appendRows(newScreen);
    appendBuffer(newScreen, MOVE_CURSOR(1, 1));
    appendBuffer(newScreen, SHOW_CURSOR);
    write(STDOUT_FILENO, getBufferString(newScreen), getBufferLen(newScreen));
    destroyBuffer(newScreen);
}