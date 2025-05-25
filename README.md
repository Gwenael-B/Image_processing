# <p align="center">**Image processing en C**</p>
____
### 1. Présentation du projet
Programme de traitement d'image en C. Le but du programme est de lire des images BMP, de transformer ces images et de les sauvegarder dans des nouveaux fichiers BMP. 
Le tout est géré par un menu simple et intuitif.

### 2. Instructions pour utiliser le programme
Ouvrir Clion, depuis l'écran d'accueil, cliquez sur "Get from Version Control" (ou allez dans File > Project from Version Control > Git). 
Dans le champ "URL", collez l'URL de votre repository GitHub que vous avez copiée, sélectionnez un dossier local où clonez le projet et cliquez sur "Clone".
Il faut maintenant compiler le programme (processus qui transforme le code source en langage machine). Pour cela il faut cliquer sur le symbole 
de marteau en haut à droite à côté du symbole de flèche. Une fois la compilation effectué, on peut lancer le programme. Il faut cliquer sur le symbole de flèche en haut 
à droite ou faire clic droit sur le fichier main.c et cliquer sur run.

### 3. Fonctionnalités implémentées 
*Fonctions essentiels :*
- Ouverture et sauvegarde d'images
- Affichage des informations de l'image

*Fonctions de transformations d'images :*
- Inversion des couleurs 
- Modification de la luminosité 
- Transformation en image binaires (seulement les images en noir et blanc)
- Conversion en niveaux de gris (seulement les images en couleurs)

*Fonctions de filtre d'images :*
- Filtre de flou 
- Filtre de flou gaussien
- Filtre de détection de contours 
- Filtre de relief 
- Filtre de netteté

*Fonctions d'égalisation :*
- Egalisation par historigramme 

### 4. Bugs connus 
- Problème si on copie un texte comprenant des retours à la ligne