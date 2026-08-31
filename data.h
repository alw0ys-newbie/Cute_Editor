#ifndef DATA
#define DATA

#include <stdlib.h>
#include <termios.h>

/*** editorConfig ***/

/* Describes the actual configuration of the editor*/
typedef struct editorConfig {
    int rows;
    int cols;
    struct termios orig_termios;
} editorConfig;

/*** textBuffer***/

/* Pointer to a textBuffer struct */
typedef struct textBuffer* textBuffer_t;

/* Initiates a pointer to a textBuffer instance*/
textBuffer_t initBuffer();
void appendBuffer(textBuffer_t tB, char* string);
void destroyBuffer(textBuffer_t tB);
char* getBufferString(textBuffer_t tB);
size_t getBufferLen(textBuffer_t tB);

#endif