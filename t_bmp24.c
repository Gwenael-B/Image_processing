#include "t_bmp24.h"
#include "t_bmp24.h"

// 2.3 Fonctions d’allocation et de libération de mémoire

t_pixel ** bmp24_allocateDataPixels (int width, int height) {

}

void bmp24_freeDataPixels (t_pixel ** pixels, int height) {

}

t_bmp24 * bmp24_allocate (int width, int height, int colorDepth) {

}

void bmp24_free (t_bmp24 * img) {

}

// 2.4 Fonctionnalités : Lecture et écriture d’image 24 bits

t_bmp24 * bmp24_loadImage (const char * filename) {

}

void bmp24_saveImage (t_bmp * img, const char * filename) {

}

void file_rawRead (uint32_t position, void * buffer, uint32_t size, size_t n, FILE * file) {
    fseek(file, position, SEEK_SET);
    fread(buffer, size, n, file);
}

void file_rawWrite (uint32_t position, void * buffer, uint32_t size, size_t n, FILE * file) {
    fseek(file, position, SEEK_SET);
    fwrite(buffer, size, n, file);
}

void bmp24_readPixelValue (t_bmp * image, int x, int y, FILE * file) {

}
void bmp24_readPixelData (t_bmp * image, FILE * file) {

}

void bmp24_writePixelValue (t_bmp * image, int x, int y, FILE * file) {

}
void bmp24_writePixelData (t_bmp * image, FILE * file) {

}

//2.5 Fonctionnalités : Traitement d’image 24 bits

void bmp24_negative (t_bmp * img) {

}

void bmp24_grayscale (t_bmp * img) {

}

void bmp24_brightness (t_bmp * img, int value) {

}

//2.6 Fonctionnalités : Filtres de convolution

t_pixel bmp24_convolution (t_bmp24 * img, int x, int y, float ** kernel, int kernelSize) {

}