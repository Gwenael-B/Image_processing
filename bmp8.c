#include <stdio.h>
#include <stdlib.h>
#include "bmp8.h"
//
t_bmp8 * bmp8_loadImage(const char * filename){
  //Ouverture du fichier
  FILE * file;
  file = fopen(filename, "r");
  //Vérification de la bonne ouverture du fichier
  if (file == NULL) {
    printf("Erreur ! impossible d'ouvrir le fichier %s\n", filename);
    return NULL;
  }
  unsigned char header[54];
  // Vérification de la profondeur de couleur
  unsigned int colorDepth = *(unsigned int*)&header[28];
  if (colorDepth != 8){
    printf("Erreur ! L'image n'a pas une profondeur de couleur de 8 bits.\n");
    fclose(file);
    return NULL;
  }
  //Allocation et initialisation de la structure t_bmp8 image
  t_bmp8* image = (t_bmp8*)malloc(sizeof(t_bmp8));
  image->width = *(unsigned int*)&header[18];
  image->height = *(unsigned int*)&header[22];
  image->colorDepth = colorDepth;
  image->dataSize = *(unsigned int*)&header[34];
  fclose(file);
  printf("Image chargée avec succès !\n");
  return image;
}

void bmp8_saveImage(const char * filename, t_bmp8 * img) {
  FILE * file;
  file = fopen(filename, "wb");
  if (file == NULL) {
    printf("Erreur lors de la sauvegarde");
  }
  else {
    unsigned char header[54] = {0};
    header[0] = 'B';
    header[1] = 'M';
    *(unsigned int*)&header[2] = 54 + 1024 +img->dataSize;
    *(unsigned int*)&header[4] = 0;
    *(unsigned int*)&header[14] = 40;
    *(unsigned int*)&header[18] = img->width;
    *(unsigned int*)&header[22] = img->height;
    *(unsigned int*)&header[26] = 1;
    *(unsigned int*)&header[28] = img->colorDepth;
    *(unsigned int*)&header[30] = img->dataSize;
  }
  fclose(file);
}

void bmp8_free(t_bmp8 * img){
  if (img != NULL) {
    free(img);
  }
}

void bmp8_printInfo(t_bmp8 * img){
    printf("Image info :");
    printf("\n  Width : %d\n  Height : %d\n  Color Depth : %d\n  Date Size : %d", img->width, img->height, img->colorDepth, img->dataSize);
  }

void bmp8_negative(t_bmp8 * img) {
  if (img == NULL) {
    printf("Erreur");
  }
  else {
    for (int i = 0; i < img->width * img->height; i++) {
      img->data[i] = 255 - img->data[i];
    }
  }
}

void bmp8_brightness(t_bmp8 * img, int value) {
  if (img == NULL) {
    printf("Erreur");
  }
  else {
    for (int i = 0; i < img->width * img->height; i++) {
      img->data[i] = img->data[i] + value;
      if (img->data[i] > 255) {
        img->data[i] = 255;
      }
      if (img->data[i] < 0) {
        img->data[i] = 0;
      }
    }
  }
}

void bmp8_threshold(t_bmp8 * img, int threshold) {
  if (img == NULL) {
    printf("Erreur");
  }
  else {
    for (int i = 0; i < img->width * img->height; i++) {
      if (img->data[i] > threshold) {
        img->data[i] = 255;
      }
      else {
        img->data[i] = 0;
      }
    }
  }
}