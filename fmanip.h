#ifndef FMANIP
#define FMANIP

struct flines {
    int nbLines;
    int maxLines;
    char** linesList;
};

int parse_file(char* fpath, int lineLength, struct flines* flines_t);
int free_flines(struct flines* flines_t);
#endif