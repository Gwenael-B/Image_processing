#include "bmp8.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    t_bmp8 * img;
    int choix;
    choix:
    printf("Veuillez choisir une option :\n  1. Ouvrir une image\n  2. Sauvegarder une image\n  3. Appliquer un filtre\n  4. Afficher les informationds de l'image\n  5. Quitter\n");
    scanf("%d", &choix);
    if ((choix != 1) && (choix != 2) && (choix != 3) && (choix != 4) && (choix != 5)) {
        do {
            printf("Erreur, veuillez entrer un nombre correct : ");
            scanf("%d", &choix);
        } while ((choix != 1) && (choix != 2) && (choix != 3) && (choix != 4) && (choix != 5));
    }
    printf(">>> Votre choix : %d", choix);
    char chemin[50];
    if (choix == 1) {
        printf("\nChemin du fichier : ");
        scanf("%s", chemin);
        printf("Image chargée avec succès !\n");
        goto choix;
    }
    if (choix == 2) {
        printf("\nChemin du fichier : ");
        scanf("%s", chemin);
        printf("Image sauvegardée avec succès !");
        goto choix;
    }
    if (choix == 3) {
        printf("Veuillez choisir un filtre :\n  1. Négatif\n    2. Luminosité\n 3. Binarisation\n   4. Flou\n   5. Flou gaussien\n  5. Netteté\n    6. Contours\n   7. Relief\n 8. Retourner au menu précédent");
        scanf("%d", &choix);
        if ((choix != 1) && (choix != 2) && (choix != 3) && (choix != 4) && (choix != 5) && (choix != 6) && (choix != 7) && (choix != 8)) {
            do {
                printf("Erreur, veuillez entrer un nombre correct : ");
                scanf("%d", &choix);
            } while ((choix != 1) && (choix != 2) && (choix != 3) && (choix != 4) && (choix != 5) && (choix != 6) && (choix != 7) && (choix != 8));
        }
        printf(">>> Votre choix : %d", choix);
        if (choix == 1) {
            bmp8_negative(img);
        }
        if (choix == 2) {
            int value;
            printf("Veuillez choisir une valeur : ");
            scanf("%d", &value);
            bmp8_brightness(img, value);
        }
        if (choix == 3) {
            bmp8_threshold(img, 128);
        }
        if (choix == 4){}
        if (choix == 5){}
        if (choix == 6){}
        if (choix == 7){}
        goto choix;
    }
    if (choix == 4) {
        bmp8_printInfo(img);
        goto choix;
    }

    return 0;
}
