#include "bmp8.h"
#include "t_bmp24.h"
#include <stdio.h>
#include <stdlib.h>

void sanitize_fielname(char s[]) {
    int len=strlen(s);
    int i;

    i=len-1;
    while (i>=0 && (s[i])) {
        s[i]='\0';
        i--;
    }
}

void menu_choix3 (t_bmp8 * img) {
    int choix = 0;
    float **kernel = malloc(3 * sizeof(float*));

    while (choix ==0) {
        printf("Veuillez choisir un filtre :\n 1. Négatif\n 2. Luminosité\n 3. Binarisation\n 4. Flou\n 5. Flou gaussien\n 6. Netteté\n 7. Contours\n 8. Relief\n 9. Retourner au menu précédent\nVotre choix :");
        scanf("%d", &choix);
        switch (choix) {
            case 1:
                bmp8_negative(img);
            break;
            case 2:
                int value;
            printf("Veuillez choisir une valeur : ");
            scanf("%d ", &value);
            bmp8_brightness(img, value);
            break;
            case 3:
                bmp8_threshold(img, 128);
            break;
            case 4:
                for (int i = 0; i < 3; i++) {
                    kernel[i] = malloc(3 * sizeof(float));
                    for (int j = 0; j < 3; j++) {
                        kernel[i][j] = 1.0f / 9.0f; // Flou simple
                    }
                }
                    bmp8_applyFilter(img, kernel, 9);
                break;
            case 5:
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
                bmp8_applyFilter(img, kernel, 9);
                break;
            case 6:
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
                bmp8_applyFilter(img, kernel, 9);
                break;
            case 7:
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
                bmp8_applyFilter(img, kernel, 9);
                break;
            case 8:
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
                bmp8_applyFilter(img, kernel, 9);
                break;
            case 9:
                return;
            default:
                printf("Erreur, veuillez entrer un nombre correct.");
        }
    }
    free(kernel);
}

void menu_choix3_24 (t_bmp24 * img) {
    int choix = 0;
    while (choix ==0) {
        printf("Veuillez choisir un filtre :\n 1. Négatif\n 2. Luminosité\n 3. valeurs de gris\n 4. Flou\n 5. Flou gaussien\n 6. Netteté\n 7. Contours\n 8. Relief\n 9. Retourner au menu précédent\nVotre choix :");
        scanf("%d", &choix);
        switch (choix) {
            case 1:
                bmp24_negative(img);
                break;
            case 2:
                int value;
                printf("Veuillez choisir une valeur : ");
                scanf("%1d ", &value);
                bmp24_brightness(img, value);
                break;
            case 3:
                bmp24_grayscale(img);
                break;
            case 4:
                bmp24_boxBlur(img);
                break;
            case 5:
                bmp24_gaussianBlur(img);
                break;
            case 6:
                bmp24_sharpen(img);
                break;
            case 7:
                bmp24_outline(img);
                break;
            case 8:
                bmp24_emboss(img);
                break;
            case 9:
                return;
            default:
                printf("Erreur, veuillez entrer un nombre correct.");
        }
    }
}

void menu_bmp8() {
    t_bmp8 * img;
    int choix;
    char chemin[255];
    while (1==1) {
        printf("\nVeuillez choisir une option :\n  1. Ouvrir une image\n  2. Sauvegarder une image\n  3. Appliquer un filtre\n  4. Afficher les informations de l'image 5. Egalisation par histogramme\n  6. Quitter\n Votre choix : ");
        scanf("%d", &choix);
        switch (choix) {
            case 1:
                printf("Chemin du fichier : ");
                scanf("%256s", chemin); //  on limite la taille de l'input à 256 caractères
                chemin[strcspn(chemin, "\n")] = 0;
                while (getchar() != '\n')

                img = bmp8_loadImage(chemin);
                break;
            case 2:
                printf("\nChemin du fichier : ");
                scanf("%s", chemin);
                bmp8_saveImage(chemin,img);
                break;
            case 3:
                menu_choix3(img);
                break;
            case 4:
                bmp8_printInfo(img);
                break;
            case 5:
                unsigned int* hist = bmp8_computeHistogram(img);
                unsigned int* hist_eq = bmp8_computeCDF(hist);
                bmp8_equalize(img,hist_eq);
                break;
            case 6:
                return ;
            default:
                printf("Erreur, veuillez entrer un nombre correct.\n");
        }
    }
}

void menu_bmp24() {
    t_bmp24 * img;
    int choix;
    char chemin[255];
    while (1==1) {
        printf("\nVeuillez choisir une option :\n  1. Ouvrir une image\n  2. Sauvegarder une image\n  3. Appliquer un filtre\n  4. Afficher les informations de l'image\n  5. Egaliser image\n  6. Quitter\n Votre choix : ");
        scanf("%d", &choix);
        switch (choix) {
            case 1:
                printf("Chemin du fichier : ");
                scanf("%s", chemin);

                img = bmp24_loadImage(chemin);
                break;
            case 2:
                printf("\nChemin du fichier : ");
                scanf("%s", chemin);
                bmp24_saveImage(img,chemin);
                break;
            case 3:
                menu_choix3_24(img);
                break;
            case 4:
                printf("\nImage info :\n   Width : %d\n   height : %d\n   profondeur couleurs : %d bits\n   Resolution X : %d\n   Resolution X : %d\n   compression : %d\n ",img->header_info.width,img->header_info.height,img->header_info.bits,img->header_info.xresolution,img->header_info.yresolution,img->header_info.compression );
                break;
            case 5:
                unsigned int* hist = bmp24_computeHistogram(img);
                unsigned int* hist_eq = bmp24_computeCDF(hist);
                bmp24_equalize(img,hist_eq);
                break;
             case 6:
                return;
            default:
                printf("Erreur, veuillez entrer un nombre correct.\n");
        }
    }
}

int main(void) {
    int choix;
    printf("\nVeuillez choisir une option :\n  1. Traiter une image bmp 8 bits\n  2. Traiter une image bmp 24 bits\n  3. Quitter\n Votre choix : ");
    scanf("%d", &choix);
       switch (choix) {
            case 1:
                menu_bmp8();
                break;
            case 2:
                menu_bmp24();
                break;
            case 3:
                return 0;
            default:
                printf("Erreur, veuillez entrer un nombre correct.\n");
        }
   }

