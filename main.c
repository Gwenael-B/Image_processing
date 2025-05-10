#include "bmp8.h"
#include "t_bmp24.h"
#include <stdio.h>
#include <stdlib.h>

void menu_choix3 (t_bmp8 * img) {
    int choix = 0;
    float **kernel = malloc(3 * sizeof(float*));
    while (choix ==0) {
        printf("Veuillez choisir un filtre :\n 1. Négatif\n 2. Luminosité\n 3. Binarisation\n 4. Flou\n 5. Flou gaussien\n 6. Contours\n 7. Relief\n 8. Netteté\n Votre choix :");
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
                        kernel[i][j] = 1.0f/9.0f; // Flou simple
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
        printf("\nVeuillez choisir une option :\n  1. Ouvrir une image\n  2. Sauvegarder une image\n  3. Appliquer un filtre\n  4. Afficher les informations de l'image\n  5. Quitter\n Votre choix : ");
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

