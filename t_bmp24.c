#include "t_bmp24.h"

// 2.3 Fonctions d’allocation et de libération de mémoire

t_pixel ** bmp24_allocateDataPixels (int width, int height) {
    t_pixel** pixels = malloc(height * sizeof(t_pixel*));
    for (int i = 0; i < height; i++) {
        pixels[i] = malloc(width * sizeof(t_pixel));
    }
    if (!pixels) {
        printf("Failed to allocate memory for pixels\n");
        return  NULL;
    }
    return pixels;

}

void bmp24_freeDataPixels (t_pixel ** pixels, int height) {
    for (int i = 0; i < height; i++) {
        free(pixels[i]);
    }
    free(pixels);
}

t_bmp24 * bmp24_allocate (int width, int height, int colorDepth) {
    t_bmp24 *img = (t_bmp24*) malloc(sizeof(t_bmp24));
    img->data = (t_pixel **) bmp24_allocateDataPixels(width, height);
    img->width = width;
    img->height = height;
    img->colorDepth = colorDepth;
    img->header=* (t_bmp_header*) &img->header;
    img->header_info=*(t_bmp_info*) &img->header_info;
    return img;
}

void bmp24_free (t_bmp24 * img) {
    bmp24_freeDataPixels(img->data, img->height);
    free(img);

}

// 2.4 Fonctionnalités : Lecture et écriture d’image 24 bits

/*
* @brief Positionne le curseur de fichier à la position position dans le fichier file,
* puis lit n éléments de taille size dans buffer.
* @param position La position à partir de laquelle il faut lire dans file.
* @param buffer Le buffer pour stocker les données lues.
* @param size La taille de chaque élément à lire.
* @param n Le nombre d'éléments à lire.
* @param file Le descripteur de fichier dans lequel il faut lire.
*/
void file_rawRead (uint32_t position, void * buffer, uint32_t size, size_t n, FILE * file) {
    fseek(file, position, SEEK_SET);
    fread(buffer, size, n, file);
}

/*
* @brief Positionne le curseur de fichier à la position position dans le fichier file,
* puis écrit n éléments de taille size depuis le buffer.
* @param position La position à partir de laquelle il faut écrire dans file.
* @param buffer Le buffer contenant les éléments à écrire.
* @param size La taille de chaque élément à écrire.
* @param n Le nombre d'éléments à écrire.
* @param file Le descripteur de fichier dans lequel il faut écrire.
* @return void
*/
void file_rawWrite (uint32_t position, void * buffer, uint32_t size, size_t n, FILE * file) {
    fseek(file, position, SEEK_SET);
    fwrite(buffer, size, n, file);
}

void bmp24_readPixelValue (t_bmp24 * image, int x, int y, FILE * file) {

}
void bmp24_readPixelData (t_bmp24 * image, FILE * file) {

}

void bmp24_writePixelValue (t_bmp24 * image, int x, int y, FILE * file) {

}
void bmp24_writePixelData (t_bmp24 * image, FILE * file) {

}
t_bmp24 * bmp24_loadImage (const char * filename) {
    FILE *file = fopen(filename, "rb");
    if(!file){
        printf("Erreur, l'image sélectionnée n'est pas correcte.\n");
        return NULL;
    }
    t_bmp_header header;
    file_rawRead(BITMAP_MAGIC, &header, HEADER_SIZE, 1, file);
    t_bmp_info header_info;
    file_rawRead(HEADER_SIZE, &header_info, INFO_SIZE, 1, file);

    //bmp24_allocate()
}

void bmp24_saveImage ( t_bmp24* img, const char * filename) {

}



//2.5 Fonctionnalités : Traitement d’image 24 bits

void bmp24_negative (t_bmp24 * img) {

}

void bmp24_grayscale (t_bmp24 * img) {

}

void bmp24_brightness (t_bmp24 * img, int value) {

}

//2.6 Fonctionnalités : Filtres de convolution

t_pixel bmp24_convolution (t_bmp24 * img, int x, int y, float ** kernel, int kernelSize) {

}