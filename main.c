#include "bmp8.h"
#include "t_bmp24.h"
#include <stdio.h>
#include <stdlib.h>

void menu_choix3 (t_bmp8 * img) {
    int choix = 0;
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
                float **kernel = malloc(3 * sizeof(float*));
                for (int i = 0; i < 3; i++) {
                    kernel[i] = malloc(3 * sizeof(float));
                    for (int j = 0; j < 3; j++) {
                        kernel[i][j] = 1.0f / 9.0f; // Flou simple
                    }
                }
                    bmp8_applyFilter(img, kernel, 9);
                break;
            case 5:
                break;
            case 6:
                break;
            case 7:
                break;
            case 8:
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
        printf("\nVeuillez choisir une option :\n  1. Ouvrir une image\n  2. Sauvegarder une image\n  3. Appliquer un filtre\n  4. Afficher les informations de l'image\n  5. Egalisation par histogramme\n  6. Quitter\n Votre choix : ");
        scanf("%d", &choix);
        switch (choix) {
            case 1:
                printf("Chemin du fichier : ");
                scanf("%s", chemin);
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
        printf("\nVeuillez choisir une option :\n  1. Ouvrir une image\n  2. Sauvegarder une image\n  3. Appliquer un filtre\n  4. Afficher les informations de l'image\n  5. Quitter\n Votre choix : ");
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
                //menu_choix3(img);
                break;
            case 4:
                /*rien */
                break;
            case 5:
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