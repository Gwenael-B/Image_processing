#include "t_bmp24.h"
#include <math.h>

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
void file_rawRead (long const position, void * buffer, uint32_t const size, size_t const n, FILE * file) {
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
    //calcul position
    fseek(file, image->header.offset+((((image->header_info.height-x)*image->header_info.width)+y)*3), SEEK_SET);
    fread(&image->data[x][y], sizeof(t_pixel), 1, file);

}
void bmp24_readPixelData (t_bmp24 * image, FILE * file) {
    for (int i = image->height-1; i >=0;  i--) {
        for (int j = 0; j < image->width;  j++) {
            bmp24_readPixelValue(image,i,j,file);
        }
    }
}

void bmp24_writePixelValue (t_bmp24 * image, int x, int y, FILE * file) {
    fseek(file, image->header.offset+((((image->header_info.height-x)*image->header_info.width)+y)*3), SEEK_SET);
    fwrite(&image->data[x][y], sizeof(t_pixel), 1, file);
}

void bmp24_writePixelData (t_bmp24 * image, FILE * file) {
    fseek(file, image->header.offset,SEEK_SET);
    for (int i = image->height-1; i >=0;  i--) {
        for (int j = 0; j < image->width;  j++) {
            fwrite(&image->data[i][j], sizeof(t_pixel), 1, file);
        }
    }
}

t_bmp24 * bmp24_loadImage (const char * filename) {
    FILE *file = fopen(filename, "rb");
    if(!file){
        printf("Erreur, l'image sélectionnée n'est pas correcte.\n");
        return NULL;
    }
    t_bmp_header *header = (t_bmp_header *) malloc(sizeof(t_bmp_header));
    t_bmp_info *header_info = (t_bmp_info *) malloc(sizeof(t_bmp_info));

    file_rawRead(BITMAP_WIDTH,&header_info->width,sizeof(header_info->width),1,file);
    file_rawRead(BITMAP_HEIGHT,&header_info->height,sizeof(header_info->height),1,file);

    if (header_info->width %4 !=0  || header_info->height %4 != 0) {
        printf("Erreur, les dimensions de l'image sélectionnée ne sont pas des mutliples de 4.\n");
        fclose(file);
        return NULL;
    }

    file_rawRead(BITMAP_DEPTH,&header_info->bits,sizeof(header_info->bits),1,file);

    if (header_info->bits !=24 ) {
        printf("Erreur, Impossible de charger l'image elle n'est pas 24 bits.\n");
        fclose(file);
        return NULL;
    }
    file_rawRead(BITMAP_MAGIC, header,sizeof(t_bmp_header) , 1, file);

    file_rawRead(HEADER_SIZE, header_info, INFO_SIZE, 1, file);
    t_bmp24* img = bmp24_allocate(header_info->width,header_info->height,header_info->bits);
    img->header=*(t_bmp_header *) header;
    img->header_info=*(t_bmp_info *) header_info;
    img->height=img->header_info.height;
    img->width=img->header_info.width;

    bmp24_readPixelData(img,file);
    fclose(file);
    return img;
}

void bmp24_saveImage ( t_bmp24* img, const char * filename) {
    FILE *file = fopen(filename, "wb");//Ecrit en binaire
    if (!file) {
        printf("Erreur, création du fichier %s impossible.\n", filename);
        return;
    }

    //Ecrit le header
    file_rawWrite(BITMAP_MAGIC,&img->header, sizeof(t_bmp_header),1, file);

    //Ecrit le header info de l'image
    file_rawWrite(HEADER_SIZE, &img->header_info, INFO_SIZE, 1, file);

    //Ecrit les données de l'image
    bmp24_writePixelData(img,file);

    //Ferme le fichier
    fclose(file);
    printf("Image  %s sauvegardée avec succès !",filename);

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
        printf(" L'image est vide, impossible de faire sa conversion en niveaux de gris.\n");
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
            img->data[y][x].red = (img->data[y][x].red + value>255) ? 255 : img->data[y][x].red + value;
            img->data[y][x].green = (img->data[y][x].green + value > 255) ? 255 : img->data[y][x].green + value;
            img->data[y][x].blue = (img->data[y][x].blue + value > 255) ? 255 : img->data[y][x].blue + value;
        }
    }
}

//2.6 Fonctionnalités : Filtres de convolution
t_pixel bmp24_convolution (t_bmp24 * img, int x, int y, float ** kernel, int kernelSize) {
    int  offset = (int)sqrt(kernelSize) / 2;
    t_pixel* pixel  = malloc(sizeof(t_pixel));


    float red = 0;
    float green = 0;
    float blue = 0;

    for (int i = -offset; i <= offset; i++) {
        for (int j = -offset; j <= offset; j++) {
            blue += img->data[x + j][y + i].blue * kernel[j + offset][i + offset];
            red += img->data[x + j][y + i].red * kernel[j + offset][i + offset];
            green += img->data[x + j][y + i].green * kernel[j + offset][i + offset];
        }
    }

    // Entre 0 et 255

    if (red<0) {pixel->red = 0;}
    else if (red>255) {pixel->red = 255;}
    else { pixel->red =(unsigned int)red;}


    if (green<0) {pixel->green = 0;}
    else if (green>255) {pixel->green = 255;}
    else { pixel->green = (unsigned int)green;}

    if (blue<0) {pixel->blue = 0;}
    else if (blue>255){ pixel->blue = 255;}
    else { pixel->blue = (unsigned int)blue;}

    return *pixel;
}

void bmp24_applyFilter(t_bmp24 * img, float ** kernel, int kernelSize) {
    int  offset = (int)sqrt(kernelSize) / 2;

    t_bmp24 * img_original = malloc(sizeof(t_bmp24));
    img_original->height=img->height;

    //Allouer de la mémoire pour une copie de l'image originale
    t_pixel ** original = bmp24_allocateDataPixels(img->width, img->height);
    for (int i = 0; i < img->height; i++) {
        for (int j = 0; j < img->width; j++) {
            original[j][i] = img->data[j][i];
        }
    }
    img_original->data=original;

    // Appliquer le filtre (on ne traite pas les bords)
    for (int y = offset; y < img->height - offset; y++) {
        for (int x = offset; x < img->width - offset; x++) {
            img->data[x][y] = bmp24_convolution(img,x,y,kernel,kernelSize);
        }
    }
    free(kernel);
    bmp24_free (img_original);
}


void bmp24_gaussianBlur(t_bmp24 * img) {
    int kernelSize=9;
    float **kernel = malloc(3 * sizeof(float*));
    for (int i = 0; i < 3; i++) {
        kernel[i] = malloc(3 * sizeof(float));

    }
    kernel[0][0] = 1.0f/16.0f;
    kernel[0][1] = 2.0f/16.0f;
    kernel[0][2] = 1.0f/16.0f;
    kernel[1][0] = 2.0f/16.0f;
    kernel[1][1] = 4.0f/16.0f;
    kernel[1][2] = 2.0f/16.0f;
    kernel[2][0] = 1.0f/16.0f;
    kernel[2][1] = 2.0f/16.0f;
    kernel[2][2] = 1.0f/16.0f;

    bmp24_applyFilter(img,kernel,kernelSize);
} //Appliquer un filtre de flou gaussien à l’image.
void bmp24_outline(t_bmp24 * img) {
    int kernelSize=9;
    float **kernel = malloc(3 * sizeof(float*));
    for (int i = 0; i < 3; i++) {
        kernel[i] = malloc(3 * sizeof(float));
    }
    kernel[0][0] = -1.0f;
    kernel[0][1] = -1.0f;
    kernel[0][2] = -1.0f;
    kernel[1][0] = -1.0f;
    kernel[1][1] = 8.0f;
    kernel[1][2] = -1.0f;
    kernel[2][0] = -1.0f;
    kernel[2][1] = -1.0f;
    kernel[2][2] = -1.0f;
    bmp24_applyFilter(img,kernel,kernelSize);
} // Appliquer un filtre de détection de contours à l’image.

void bmp24_emboss(t_bmp24 * img) {
    int kernelSize=9;
    float **kernel = malloc(3 * sizeof(float*));
    for (int i = 0; i < 3; i++) {
        kernel[i] = malloc(3 * sizeof(float));
    }
    kernel[0][0] = -2.0f;
    kernel[0][1] = -1.0f;
    kernel[0][2] = 0.0f;
    kernel[1][0] = -1.0f;
    kernel[1][1] = 1.0f;
    kernel[1][2] = 1.0f;
    kernel[2][0] = 0.0f;
    kernel[2][1] = 1.0f;
    kernel[2][2] = 2.0f;
    bmp24_applyFilter(img,kernel,kernelSize);
} // Appliquer un filtre de relief à l’image.

void bmp24_sharpen(t_bmp24 * img) {
    int kernelSize=9;
    float **kernel = malloc(3 * sizeof(float*));
    for (int i = 0; i < 3; i++) {
        kernel[i] = malloc(3 * sizeof(float));
    }
    kernel[0][0] = 0.0f;
    kernel[0][1] = -1.0f;
    kernel[0][2] = 0.0f;
    kernel[1][0] = -1.0f;
    kernel[1][1] = 5.0f;
    kernel[1][2] = -1.0f;
    kernel[2][0] = 0.0f;
    kernel[2][1] = -1.0f;
    kernel[2][2] = 0.0f;
    bmp24_applyFilter(img,kernel,kernelSize);
}

void bmp24_boxBlur(t_bmp24 * img) {    // Appliquer un filtre de flou à l’image.
    int kernelSize=9;
    float **kernel = malloc(3 * sizeof(float*));
    for (int i = 0; i < 3; i++) {
        kernel[i] = malloc(3 * sizeof(float));
        for (int j = 0; j < 3; j++) {
            kernel[i][j] = 1.0f / 9.0f; // Flou simple
        }
    }
    bmp24_applyFilter(img,kernel,kernelSize);
}

void bmp24_equalize(t_bmp24 * img,const unsigned int * hist_eq) {
    for (int i =0 ;i< img->height;i++) {
        for (int j =0 ;j< img->width;j++) {
            img->dataYUV[i][j].y = hist_eq[img->dataYUV[i][j].y];
            //on convertie en  rgb à partir de yuv
            img->data[i][j].red = (unsigned short)(img->dataYUV[i][j].y + 1.402 * (img->dataYUV[i][j].v - 128));
            img->data[i][j].green = (unsigned short)(img->dataYUV[i][j].y - 0.344136 * (img->dataYUV[i][j].u - 128) - 0.714136 * (img->dataYUV[i][j].v - 128));
            img->data[i][j].blue = (unsigned short)(img->dataYUV[i][j].y + 1.772 * (img->dataYUV[i][j].u - 128));

            // S'assurer que les valeurs de RGB sont dans la plage [0, 255]
            if (img->data[i][j].red > 255) img->data[i][j].red = 255;
            if (img->data[i][j].green > 255) img->data[i][j].green = 255;
            if (img->data[i][j].blue > 255) img->data[i][j].blue = 255;

            if (img->data[i][j].red < 0) img->data[i][j].red = 0;
            if (img->data[i][j].green < 0) img->data[i][j].green = 0;
            if (img->data[i][j].blue < 0) img->data[i][j].blue = 0;

       /*    img->data[i][j].red = (unsigned int)round(img->dataYUV[i][j].y+ (img->dataYUV[i][j].v * 1.13983f));
            img->data[i][j].green =  (unsigned int)round(img->dataYUV[i][j].y-(0.39465f * img->dataYUV[i][j].u) - (0.58060f * img->dataYUV[i][j].v));
            img->data[i][j].blue = (unsigned int)round(img->dataYUV[i][j].y+(2.03211*img->dataYUV[i][j].u));*/
        }
    }
}

unsigned int * bmp24_computeHistogram(t_bmp24 * img) {
    static unsigned int hist [256] = {0};
    t_pixel_yuv ** dataYUV = malloc(img->height * sizeof(t_pixel_yuv*));
    for (int i = 0; i < img->height; i++) {
        dataYUV[i] = malloc(img->width * sizeof(t_pixel_yuv));
    }

    for (int i =0 ;i< img->height;i++) {
        for (int j =0 ;j< img->width;j++) {
            // Conversion RGB -> YUV
            dataYUV[i][j].y = (unsigned short)(0.299 * img->data[i][j].red + 0.587 * img->data[i][j].green + 0.114 * img->data[i][j].blue);
            dataYUV[i][j].u = (unsigned short)(-0.14713 * img->data[i][j].red - 0.28886 * img->data[i][j].green + 0.436 * img->data[i][j].blue);
            dataYUV[i][j].v = (unsigned short)(0.615 * img->data[i][j].red - 0.51499 * img->data[i][j].green - 0.10001 * img->data[i][j].blue);

            // Ajuster les valeurs de Y, U, V à une plage de 0 à 255
            if (dataYUV[i][j].y > 255) dataYUV[i][j].y = 255;
            if (dataYUV[i][j].u > 255) dataYUV[i][j].u = 255;
            if (dataYUV[i][j].v > 255) dataYUV[i][j].v = 255;
            //on convertie en  rgb à partir de yuv
            /*
            dataYUV[i][j].y=(int)round(0.299f * img->data[i][j].red+ 0.587 *img->data[i][j].green + 0.114f * img->data[i][j].blue);
            dataYUV[i][j].u=(int)round (-0.14713f * img->data[i][j].red -(0.28886f *img->data[i][j].green) + 0.436f * img->data[i][j].blue);
            dataYUV[i][j].v=(int)round((0.615f * img->data[i][j].red) - (0.515f *img->data[i][j].green) - (0.1001f * img->data[i][j].blue));*/

            hist[dataYUV[i][j].y]++;
        }
        img->dataYUV = dataYUV;
    }
    return hist;
}


unsigned int * bmp24_computeCDF(const unsigned int * hist) {
    static unsigned int hist_eq[256] = {0};
    static unsigned int cdf[256] = {0};
    cdf[0]=hist[0];
    unsigned int n = hist[0];
    unsigned int cdfMin = UINT_MAX;
    if (hist[0]>0) {
        cdfMin = hist[0];
    }

    for (int i =1 ;i < 256;i++) {
        if (hist[i]<cdfMin & hist[i]>0) {
            cdfMin = hist[i];
        }
        cdf[i]=cdf[i-1]+hist[i];
        n= n + hist[i];
    }
    printf("cdfMin = %d , n  = %d\n",cdfMin,n);
    printf("i hist cdf hist_eq\n");
    for (int i =0 ;i < 256;i++) {
        if (hist[i]!=0) {
            hist_eq[i]=(unsigned int) round((cdf[i]-cdfMin) * ( 255.0f / (n - cdfMin) ));
            printf("%d %d  %d  %d\n",i,hist[i],cdf[i],hist_eq[i]);
        }
    }
    return hist_eq;
}