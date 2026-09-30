#ifndef FMANIP
#define FMANIP

struct flines {
    int nbLines;
    int maxLines;
    char** linesList;
};

struct flines parse_file(char* fpath, int lineLength);
#endif