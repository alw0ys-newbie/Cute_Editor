#ifndef TXTBUFF
#define TXTBUFF

#include <stdlib.h>
#include <termios.h>

/*** textBuffer***/

typedef struct textBuffer* textBuffer_t;

/* Initiates a textBuffer instance and returns a pointer to it*/
textBuffer_t initBuffer();

/* Append `string` to the text buffer pointed at by `tB`*/
void appendBuffer(textBuffer_t tB, char* string);

/* Free the memory allocated to the textBuffer instance `tB`*/
void destroyBuffer(textBuffer_t tB);
char* getBufferString(textBuffer_t tB);
size_t getBufferLen(textBuffer_t tB);

#endif