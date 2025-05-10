#include <stdio.h>
#include <stdlib.h>
#include "bmp8.h"

//La fonction charge l'image d'extension bmp 8 bits
t_bmp8 * bmp8_loadImage(const char * filename){
  FILE *file = fopen(filename, "rb");
  if(!file){
    printf("Erreur, l'image sélectionnée n'est pas correcte.\n");
    return NULL;
  }

  t_bmp8 *img = (t_bmp8 *)malloc(sizeof(t_bmp8));

  if (img==NULL){
    printf("Erreur (Pas assez de mémoire pour l'image.)\n");
    fclose(file);
    return NULL;
  }

  //Lecture header
  fread(img->header, sizeof(unsigned char), 54, file);

  //Lecture table de couleurs
  fread(img->colorTable, sizeof(unsigned char), 1024, file);

  //Extraire informations du header
  img->offset = *(unsigned int*)&img->header[10];
  img->width = *(unsigned int*)&img->header[18];
  img->height = *(unsigned int*)&img->header[22];
  img->colorDepth = *(unsigned int*)&img->header[28];
  img->dataSize = img->width * img->height; //  *(unsigned int*)&img->header[34];
  img->pixels = malloc(img->height * sizeof(unsigned char*));
  // Vérifie bien image 8 bits
  if (img->colorDepth != 8) {
    printf("Erreur,pas image en 8 bits.\n");
    free(img);
    fclose(file);
    return NULL;
  }
  // Alloue la mémoire pour les données de l'image
  img->data = (unsigned char *)malloc(img->dataSize);
  if (!img->data) {
    printf("Erreur (Pas possible d'allouer la mémoire pour les données de l'image.)\n");
    free(img);
    fclose(file);
    return NULL;
  }

  //Lecture des données de l'image
  fseek(file, img->offset, SEEK_SET);
  long blocs = fread(img->data, sizeof(unsigned char), img->dataSize, file);
  //printf("blocs lus : %d\n",blocs );
  //printf("image data : %s\n",img->data );

  for (int i = 0; i < img->height; i++) {
    img->pixels[i] = malloc(img->width * sizeof(unsigned char));
  }

  printf("Image %s chargée avec succès !\n",filename);
  //Ferme le fichier et retourne l'image
  fclose(file);
  return img;
}

//La fonction sauvegarde l'image d'extension bmp 8 bits
void bmp8_saveImage(const char * filename, t_bmp8 * img) {
  FILE *file = fopen(filename, "wb");//Ecrit en binaire
  if (!file) {
    printf("Erreur, création du fichier %s impossible.\n", filename);
    return;
  }

  //Ecrit le header
  fwrite(img->header, sizeof(unsigned char), 54, file);

  //Ecrit la table de couleurs
  fwrite(img->colorTable, sizeof(unsigned char), 1024, file);

  //Ecrit les données de l'image
  fwrite(img->data, sizeof(unsigned char), img->dataSize, file);
  printf("Image  %s sauvegardée avec succès !",filename);

  //Ferme le fichier
  fclose(file);
}

//La fontion libère la mémoire
void bmp8_free(t_bmp8 * img){
  if (img) {
    if (img->data) {
      free(img->data);
    }
    free(img);
  }
}

//La fonction affiche les infos sur l'image
void bmp8_printInfo(t_bmp8 * img){
  if (!img) {
    printf("Erreur: Image invalide.\n");
    return;
  }
  printf("\nImage info :\n    Width : %d\n    Height : %d\n    Color Depth : %d\n    Date Size : %d\n", img->width, img->height, img->colorDepth, img->dataSize);
}

//La fonction change l'image en négatif : echange les couleurs
void bmp8_negative(t_bmp8 * img){
  if (!img || !img->data){
    printf(" L'image est vide, impossible de faire son negatif.\n");
    return;
  }
  for (unsigned int i = 0; i < img->dataSize; i++){
    img->data[i] = 255 - img->data[i];
  }
}

//La fonction change la luminosité l'image
void bmp8_brightness(t_bmp8 * img, int value){
  if (!img || !img->data)
    return;

  for (unsigned int i = 0; i < img->dataSize; i++){
    int pixel = img->data[i] + value;

    // Clamping (forcage dans [0, 255])
    if (pixel > 255)
      pixel = 255;
    if (pixel < 0)
      pixel = 0;

    img->data[i] = (unsigned char)pixel;
  }
}

//La fonction transforme l'image en binaire, que deux couleurs 0 ou 255
void bmp8_threshold(t_bmp8 * img, int threshold){
  if (!img || !img->data)
    return;

  for (unsigned int i = 0; i < img->dataSize; i++){
    img->data[i] = (img->data[i] >= threshold) ? 255 : 0;
  }
}

void bmp8_applyFilter(t_bmp8 * img, float ** kernel, int kernelSize){
  int width = img->width;
  int height = img->height;
  int offset = kernelSize / 2;

  // Allouer de la mémoire pour une copie de l'image originale
  unsigned char **original = malloc(height * sizeof(unsigned char *));
  for (int i = 0; i < height; i++) {
    original[i] = malloc(width * sizeof(unsigned char));
    for (int j = 0; j < width; j++) {
      original[i][j] = img->pixels[i][j]; // Copie des pixels
    }
  }
  printf("dans fonction 2");

  // Appliquer le filtre (on ne traite pas les bords)
  for (int y = offset; y < height - offset; y++) {
    for (int x = offset; x < width - offset; x++) {
      float sum = 0.0f;
      for (int i = -offset; i <= offset; i++) {
        for (int j = -offset; j <= offset; j++) {
          printf("1");
          sum += original[y + i][x + j] * kernel[i + offset][j + offset];
          printf("2");
        }
      }

      // Entre 0 et 255
      if (sum < 0) sum = 0;
      if (sum > 255) sum = 255;

      img->pixels[y][x] = (unsigned char)sum;
    }
  }

  // Libération de la mémoire temporaire
  for (int i = 0; i < height; i++) {
    free(original[i]);
  }
  free(original);
}