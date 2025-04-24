#include <stdio.h>
#include <stdlib.h>
#include "bmp8.h"

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
  return image;
}

void bmp8_free(t_bmp8 * img){
  free(img);
}

void bmp8_printInfo(t_bmp8 * img){
    printf("Image info :");
    printf("\n  Width : %d\n  Height : %d\n  Color Depth : %d\n  Date Size : %d", img->width, img->height, img->colorDepth, img->dataSize);
  }