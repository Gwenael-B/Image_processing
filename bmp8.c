#include <stdio.h>
#include <stdlib.h>
#include "bmp8.h"

t_bmp8 * bmp8_loadImage(const char * filename){
  File *fopen("lena_gray.bmp", "r");
  t_bmp8* img = malloc(sizeof(t_bmp8));
  return 0;
}

void bmp8_free(t_bmp8 * img){
  free(img);
}

void bmp8_printInfo(t_bmp8 * img){
  printf("Image info :");
  printf("\n  Width : %d\n  Height : %d\n  Color Depth : %d\n  Date Size : %d", img->width, img->height, img->colorDepth, img->dataSize);
}