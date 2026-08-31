#ifndef FILE_CUTE
#define FILE_CUTE

void die(const char* s);
void disableRawMode();
void enableRawMode();
char editorReadKey();
void editorProcessKeyPressed();
void screenRefrech();
void editorInit();
#endif