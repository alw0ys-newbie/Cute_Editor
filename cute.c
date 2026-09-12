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
#define ALTERNATIVE_SCREEN_BUFFER "\x1b[?1049h"
#define CLOSE_SCREEN_BUFFER "\x1b[?1049l"
#define ERASE_LINE_AFTER_CURSOR "\x1b[K"
#define ERASE_SCREEN "\x1b[2J"
#define HIDE_CURSOR "\x1b[?25l"
#define SHOW_CURSOR "\x1b[?25h"
#define MOVE_CURSOR(x, y) "\x1b[" #x ";" #y "f"

/*** Global Variables */

editorConfig EC;

typedef enum {
    LEFT_ARROW = 1015,
    UP_ARROW,
    RIGHT_ARROW,
    DOWN_ARROW
} Arrows;

/*** Terminal ***/

void die(const char* s)
{
    write(STDOUT_FILENO, ERASE_SCREEN, 4);
    write(STDOUT_FILENO, "\x1b[1;1f", 6);
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

void editorClose()
{
    write(STDOUT_FILENO, CLOSE_SCREEN_BUFFER, 8);
    disableRawMode();
}

void editor_init()
{
    write(STDOUT_FILENO, ALTERNATIVE_SCREEN_BUFFER, 8);
    enableRawMode();
    getWindowSize(&EC.rows, &EC.cols);
    EC.cursor_row = 1;
    EC.cursor_col = 1;
    atexit(editorClose);
}
/*** Input ***/

int editorReadKey()
{
    int nread;
    char c;
    while ((nread = read(STDIN_FILENO, &c, 1)) != 1) {
        if (nread == -1 && errno != EAGAIN) {
            die("read");
        }
    }
    if (c == '\x1b') {
        char seq[2];
        if ((nread = read(STDIN_FILENO, &seq[0], 1)) != 1)
            return '\x1b';
        if ((nread = read(STDIN_FILENO, &seq[1], 1)) != 1)
            return '\x1b';
        if (seq[0] != '[')
            return '\x1b';
        switch (seq[1]) {
        case 'A':
            return UP_ARROW;
        case 'B':
            return DOWN_ARROW;
        case 'C':
            return RIGHT_ARROW;
        case 'D':
            return LEFT_ARROW;
        }
        return '\x1b';
    }
    return c;
}

void editorProcessKeyPressed()
{
    int c;
    c = editorReadKey();

    switch (c) {
    case CTRL_KEY('q'):
        exit(0);
        break;
    case LEFT_ARROW:
        if (EC.cursor_col > 1)
            EC.cursor_col--;
        break;
    case RIGHT_ARROW:
        if (EC.cursor_col < EC.cols)
            EC.cursor_col++;
        break;
    case DOWN_ARROW:
        if (EC.cursor_row < EC.rows)
            EC.cursor_row++;
        break;
    case UP_ARROW:
        if (EC.cursor_row > 1)
            EC.cursor_row--;
        break;
    }
}

void set_cursor_position(int row, int col)
{
    char escapeSequence[25];
    size_t seqSize = snprintf(escapeSequence, sizeof(escapeSequence), "\x1b[%d;%df", row, col);
    write(STDOUT_FILENO, escapeSequence, seqSize);
}

/*** output ***/

void append_welcome_screen_text(textBuffer_t screenRows)
{
    for (int i = 0; i < EC.rows; i++) {
        if (i == EC.rows / 3) {
            char welcome[80];
            int welcomeLen = snprintf(welcome, sizeof(welcome), "C.U.T.E -- version %s", VERSION);
            if (welcomeLen > EC.cols)
                welcomeLen = EC.cols;
            int welcomeOffset = ((EC.cols - welcomeLen) / 2) + 1;
            char moveCursor[25];
            snprintf(moveCursor, sizeof(char[25]), "\x1b[%d;%df", i, welcomeOffset);
            appendBuffer(screenRows, moveCursor);
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

void write_welcome_screen()
{
    textBuffer_t newScreen = initBuffer();
    appendBuffer(newScreen, HIDE_CURSOR);
    appendBuffer(newScreen, MOVE_CURSOR(1, 1));
    append_welcome_screen_text(newScreen);
    appendBuffer(newScreen, MOVE_CURSOR(1, 1));
    appendBuffer(newScreen, SHOW_CURSOR);
    write(STDOUT_FILENO, getBufferString(newScreen), getBufferLen(newScreen));
    destroyBuffer(newScreen);
}
void screenRefrech()
{
    write_welcome_screen();
    set_cursor_position(EC.cursor_row, EC.cursor_col);
}