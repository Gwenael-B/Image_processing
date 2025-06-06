//Ce fichier contient toutes les fonctions nécessaires pour ouvrir, sauvegarder et modifier les images en noir et blanc
//L'ensemble des fonctions est utilisé dans le main
//Créé par Gwenaël et Clarence

#include <stdio.h>
#include <stdlib.h>
#include "bmp8.h"
#include <math.h>

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

  // Vérifie si la profondeur de couleur est sur 8 bits
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
  fread(img->data, sizeof(unsigned char), img->dataSize, file);
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

//La fonction change l'image en négatif : échange les couleurs
void bmp8_negative(t_bmp8 * img){
  if (!img || !img->data){
    printf(" L'image est vide, impossible de faire son negatif.\n");
    return;
  }
  //La boucle parcourt chaque pixel de l'image et inverse leur couleur
  for (unsigned int i = 0; i < img->dataSize; i++){
    img->data[i] = 255 - img->data[i];
  }
}

//La fonction change la luminosité l'image
void bmp8_brightness(t_bmp8 * img, int value){
  if (!img || !img->data)
    return;
  //La boucle parcourt chaque pixel de l'image et ajoute la valeur de luminosité à la valeur du pixel
  for (unsigned int i = 0; i < img->dataSize; i++){
    int pixel = img->data[i] + value;
    //On vérifie que les valeurs des pixels restent bien entre 0 et 255
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
  //La boucle parcourt chaque valeur des pixels de l'image, si la valeur est supérieur à celle saisie la valeur du pixel est mise à 255 sinon 0
  for (unsigned int i = 0; i < img->dataSize; i++){
    img->data[i] = (img->data[i] >= threshold) ? 255 : 0;
  }
}

void bmp8_applyFilter(t_bmp8 * img, float ** kernel, int kernelSize){
  unsigned int  width = img->width;
  unsigned int  height = img->height;
  int  offset = (int)sqrt(kernelSize) / 2;

  //Allouer de la mémoire pour une copie de l'image originale
  unsigned char **original = malloc(height * sizeof(unsigned char *));
  for (int i = 0; i < height; i++) {
    original[i] = malloc(width * sizeof(unsigned char));
    for (int j = 0; j < width; j++) {
      original[i][j] =img->data[(i*img->height)+j];
    }
  }
  // Appliquer le filtre (on ne traite pas les bords)
  for (int y = offset; y < height - offset; y++) {
    for (int x = offset; x < width - offset; x++) {
      float sum = 0.0f;
      for (int i = -offset; i <= offset; i++) {
        for (int j = -offset; j <= offset; j++) {
          sum += original[y + i][x + j] * kernel[i + offset][j + offset];
        }
      }

      // Entre 0 et 255
      if (sum < 0) sum = 0;
      if (sum > 255) sum = 255;

      img->data[(y*img->height)+x] = (unsigned char)sum;
    }
  }


  // Libération de la mémoire temporaire
  for (int i = 0; i < height; i++) {
    free(original[i]);
  }
  free(original);
}

//Uniformise la répartition des nuances de gris sur la plage  [0, 255]
void bmp8_equalize(t_bmp8 * img, unsigned int * hist_eq) {
  for (int i =0 ;i< img->dataSize;i++){
    img->data[i] = hist_eq[img->data[i]];
  }
}

//Calcul l'histogramme de la répartition des niveaux de gris par valeurs différentes
unsigned int * bmp8_computeHistogram(t_bmp8 * img) {
  static unsigned int hist [256] = {0};
  for (int i =0 ;i< img->dataSize;i++) {
    hist[img->data[i]]++;
  }
  return hist;
}

//Uniformise les niveaux de gris sur toute la plage [0, 255]
unsigned int * bmp8_computeCDF(unsigned int * hist) {
  static unsigned int hist_eq[256] = {0};
  static unsigned int cdf[256] = {0};
  cdf[0]=hist[0];
  unsigned int n = hist[0];
  unsigned int cdfMin = UINT_MAX;
  if (hist[0]>0) {
    cdfMin = hist[0];
  }

  for (int i =1 ;i < 256;i++) {
      if (hist[i]<cdfMin && hist[i]>0) {
        cdfMin = hist[i];
      }
      cdf[i]=cdf[i-1]+hist[i];
      n= n + hist[i];
  }
  for (int i =0 ;i < 256;i++) {
    if (hist[i]!=0) {
      hist_eq[i]=(unsigned int)round((cdf[i]-cdfMin) * (255.0f / (n -cdfMin) ));
    }
  }
  return hist_eq;
}