#include "t_bmp24.h"

// 2.3 Fonctions d’allocation et de libération de mémoire

t_pixel ** bmp24_allocateDataPixels (int width, int height) {
    t_pixel** pixels = malloc(height * sizeof(t_pixel*));
    if (!pixels) {
        printf("Failed to allocate memory for pixels\n");
        return  NULL;
    }
    for (int i = 0; i < height; i++) {
        pixels[i] = malloc(width * sizeof(t_pixel));
        if (!pixels[i]) {
            printf("Erreur lors de l'allocation de la mémoire pour les pixels.");
            for (int j = 0; j < i; j++) {
                free(pixels[j]);
            }
            free(pixels);
            return  NULL;
        }

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
    if (!img) {
        printf("Erreur lors de l'allocation de la mémoire pour l'image.");
        return  NULL;
    }
    img->data = bmp24_allocateDataPixels(width, height);
    img->width = width;
    img->height = height;
    img->colorDepth = colorDepth;
    img->header.type = BMP_TYPE;
    img->header.size = HEADER_SIZE + INFO_SIZE + width * height * 3;
    img->header.offset = HEADER_SIZE + INFO_SIZE;
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
    if (x<0 || x>=image->width || y<0 || y>=image->height) {
        printf("Erreur : le nombre de pixel ne correspond pas");
        return;
    }
    unsigned int position = image->header.offset + (image->height -1 -y)* image->width * 3;
    uint8_t bgr[3];
    fseek(file, position, SEEK_SET);
    fread(bgr, sizeof(uint8_t), 3, file);
    image->data[y][x].blue = bgr[0];
    image->data[y][x].green = bgr[1];
    image->data[y][x].red = bgr[2];
}

void bmp24_readPixelData (t_bmp24 * image, FILE * file) {
    for (int i=0; i < image->height; i++) {
        for (int j=0; j < image->width; j++) {
            bmp24_readPixelValue(image, j, i, file);
        }
    }
}

void bmp24_writePixelValue (t_bmp24 * image, int x, int y, FILE * file) {
    unsigned int position = image->header.offset + (image->height -1 -y)* image->width * 3;
    uint8_t bgr[3] = {image->data[y][x].blue, image->data[y][x].green, image->data[y][x].red};
    fseek(file, position, SEEK_SET);
    fwrite(bgr, sizeof(uint8_t), 3, file);
}

void bmp24_writePixelData (t_bmp24 * image, FILE * file) {
    for (int i=0; i < image->height; i++) {
        for (int j=0; j < image->width; j++) {
            bmp24_writePixelValue(image, j, i, file);
        }
    }
}

t_bmp24 * bmp24_loadImage (const char * filename) {
    FILE *file = fopen(filename, "rb");
    if(!file){
        printf("Erreur, l'image sélectionnée n'est pas correcte.\n");
        return NULL;
    }
    //lit les données du header et les stockes
    t_bmp_header header;
    t_bmp_info header_info;
    file_rawRead(BITMAP_MAGIC, &header, HEADER_SIZE, 1, file);
    file_rawRead(HEADER_SIZE, &header_info, INFO_SIZE, 1, file);

    //Alloue la mémoire nécessaire à l'image puis les sauvegarde
    t_bmp24 *image = bmp24_allocate(header_info.width, header_info.height, BITMAP_DEPTH);
    image->header = header;
    image->header_info = header_info;

    //lit enfin les données de l’image et initialise la matrice de pixels
    bmp24_readPixelData(image, file);
    fclose(file);
    return image;
}

void bmp24_saveImage ( t_bmp24* img, const char * filename) {
    FILE *file = fopen(filename, "wb");
    if(!file){
        printf("Erreur lors de la sauvegarde.\n");
        return;
    }
    //Ecris le header et ses infos
    file_rawWrite(BITMAP_MAGIC, &img->header, HEADER_SIZE, 1, file);
    file_rawWrite(HEADER_SIZE, &img->header_info, INFO_SIZE, 1, file);
    //Ecris les données de l'image
    bmp24_writePixelData(img, file);
    fclose(file);
}



//2.5 Fonctionnalités : Traitement d’image 24 bits

void bmp24_negative (t_bmp24 * img) {
    if (!img || !img->data){
        printf(" L'image est vide, impossible de faire son negatif.\n");
        return;
    }
    for (int y=0; y<img->height; y++) {
        for (int x=0; x<img->width; x++) {
            img->data[y][x].red = 255 - img->data[y][x].red;
            img->data[y][x].green = 255 - img->data[y][x].green;
            img->data[y][x].blue = 255 - img->data[y][x].blue;
        }
    }
}

void bmp24_grayscale (t_bmp24 * img) {
    if (!img || !img->data){
        printf(" L'image est vide, impossible de faire sa conversion en gris.\n");
        return;
    }
    for (int y=0; y<img->height; y++) {
        for (int x=0; x<img->width; x++) {
            int moyenne = (img->data[y][x].red + img->data[y][x].green + img->data[y][x].blue)/3;
            img->data[y][x].red = moyenne;
            img->data[y][x].green = moyenne;
            img->data[y][x].blue = moyenne;
        }
    }
}

void bmp24_brightness (t_bmp24 * img, int value) {
    if (!img || !img->data){
        printf(" L'image est vide, impossible d'ajuster le niveau de luminosité.\n");
        return;
    }
    for (int y=0; y<img->height; y++) {
        for (int x=0; x<img->width; x++) {
            img->data[y][x].red = (img->data[y][x].red + value > 255) ? 255 : img->data[y][x].red + value;
            img->data[y][x].red = (img->data[y][x].red < 0) ? 0 : img->data[y][x].red;
            img->data[y][x].green = (img->data[y][x].green + value > 255) ? 255 : img->data[y][x].green + value;
            img->data[y][x].green = (img->data[y][x].green < 0) ? 0 : img->data[y][x].green;
            img->data[y][x].blue = (img->data[y][x].blue + value > 255) ? 255 : img->data[y][x].blue + value;
            img->data[y][x].blue = (img->data[y][x].blue < 0) ? 0 : img->data[y][x].blue;
        }
    }
}

//2.6 Fonctionnalités : Filtres de convolution
t_pixel bmp24_convolution(t_bmp24 * img, int x, int y, float ** kernel, int kernelSize) {
    t_pixel pixel;


}