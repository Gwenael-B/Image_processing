//Ce fichier contient toutes les constantes servant aux fonctions pour les images en couleur
//Il contient également tous les prototypes des fonctions ainsi que les structures pour les images en couleurs
//Créé par Gwenaël et Clarence

#ifndef T_BMP24_H
#define T_BMP24_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Constantes pour les offsets des champs de l'en-tête BMP
#define BITMAP_MAGIC 0x00 // offset 0
#define BITMAP_SIZE 0x02 // offset 2
#define BITMAP_RESERVED 0x06 // offset 4
#define BITMAP_OFFSET 0x0A // offset 10
#define BITMAP_WIDTH 0x12 // offset 18
#define BITMAP_HEIGHT 0x16 // offset 22
#define BITMAP_DEPTH 0x1C // offset 28
#define BITMAP_SIZE_RAW 0x22 // offset 34

// Constante pour le type de fichier BMP
#define BMP_TYPE 0x4D42 // 'BM' en hexadécimal

// Constantes pour les tailles des champs de l'en-tête BMP
#define HEADER_SIZE 0x0E // 14 octets
#define INFO_SIZE 0x28 // 40 octets

// Constantes pour les valeurs de profondeur de couleur
#define DEFAULT_DEPTH 0x18 // 24


typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef int int32_t;


#pragma pack(1) //ajout pour ignorer le padding car la structure fait 14 bits et n'est pas un multiple de 8
typedef struct {
    uint16_t type;
    uint32_t size;
    uint32_t reserved;
    uint32_t offset;
} t_bmp_header;

typedef struct {
    uint32_t size;
    int32_t width;
    int32_t height;
    uint16_t planes;
    uint16_t bits;
    uint32_t compression;
    uint32_t imagesize;
    int32_t xresolution;
    int32_t yresolution;
    uint32_t ncolors;
    uint32_t importantcolors;
} t_bmp_info;

typedef struct {
    uint8_t blue;
    uint8_t green;
    uint8_t red;
} t_pixel;

typedef struct {
    float y;
    float u;
    float v;
} t_pixel_yuv;

typedef struct {
    t_bmp_header header;
    t_bmp_info header_info;
    int width;
    int height;
    int colorDepth;
    t_pixel **data;
    t_pixel_yuv **dataYUV;
} t_bmp24;


// 2.3 Fonctions d’allocation et de libération de mémoire
t_pixel ** bmp24_allocateDataPixels (int width, int height);
void bmp24_freeDataPixels (t_pixel ** pixels, int height);
t_bmp24 * bmp24_allocate ( int width, int height, int colorDepth);
void bmp24_free (t_bmp24 * img);

// 2.4 Fonctionnalités : Lecture et écriture d’image 24 bits
t_bmp24 * bmp24_loadImage (const char * filename);
void bmp24_saveImage (t_bmp24 * img, const char * filename);

void bmp24_readPixelValue (t_bmp24 * image, int x, int y, FILE * file);
void bmp24_readPixelData (t_bmp24 * image, FILE * file);

void bmp24_writePixelValue (t_bmp24 * image, int x, int y, FILE * file);
void bmp24_writePixelData (t_bmp24 * image, FILE * file);

// 2.5 Fonctionnalités : Traitement d’image 24 bits
void bmp24_negative (t_bmp24 * img);
void bmp24_grayscale (t_bmp24 * img);
void bmp24_brightness (t_bmp24 * img, int value);

// 2.6 Fonctionnalités : Filtres de convolution
//void bmp24_applyFilter(t_bmp24 * img, float ** kernel, int kernelSize);
t_pixel bmp24_convolution (t_bmp24 * img, int x, int y, float ** kernel, int kernelSize);

void bmp24_equalize(t_bmp24 * img,const unsigned int * hist_eq);

unsigned int * bmp24_computeHistogram(t_bmp24 * img);
unsigned int * bmp24_computeCDF(const unsigned int * hist);

void bmp24_boxBlur(t_bmp24 * img);
void bmp24_gaussianBlur(t_bmp24 * img); //Appliquer un filtre de flou gaussien à l’image.
void bmp24_outline(t_bmp24 * img); // Appliquer un filtre de détection de contours à l’image.
void bmp24_emboss(t_bmp24 * img); // Appliquer un filtre de relief à l’image.
void bmp24_sharpen(t_bmp24 * img); //

#endif //T_BMP24_H
