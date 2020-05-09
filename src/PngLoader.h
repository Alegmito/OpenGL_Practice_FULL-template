#ifndef PNG_LOADER
#define PNG_LOADER

#include <stdlib.h>

struct ImageData{
    bool loaded;
    size_t dataSize;
    char* data;
    int width;
    int height;
    bool withAlpha;

    // constructor
    ImageData(size_t inDataSize, char* inData, int inWidth, int inHeight, bool inWithAlpha);
    // destructor
    ~ImageData();
};


ImageData loadPngImage(const char* fileName);

#endif
