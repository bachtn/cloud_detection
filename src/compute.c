#include <stdio.h>
#include <gtk/gtk.h>

/*******************************************************
IL EST FORMELLEMENT INTERDIT DE CHANGER LE PROTOTYPE
DES FONCTIONS
*******************************************************/


/*---------------------------------------
  Proto: 

  
  But: 

  Entrees: 
          --->le tableau des valeurs des pixels de l'image d'origine
	      (les lignes sont mises les unes à la suite des autres)
	  --->le nombre de lignes de l'image,
	  --->le nombre de colonnes de l'image,
          --->le tableau des valeurs des pixels de l'image resultat
	      (les lignes sont mises les unes à la suite des autres)


  Sortie:

  Rem: 

  Voir aussi:

  ---------------------------------------*/
void ComputeImage(guchar *img_orig, int nb_lines, int nb_col, guchar *img_res) {
  int nb_pixels, pix_idx;
  int channel_idx, nb_channels = 3; /* on travaille sur des images couleurs*/
  guchar pix_mean;

  printf("Segmentation de l'image.... A vous!\n");
  
  nb_pixels = nb_col * nb_lines;
  for (pix_idx = 0; pix_idx < nb_pixels * nb_channels; pix_idx = pix_idx + nb_channels) {
    /*moyenne sur les composantes RVB */
    pix_mean = (unsigned char)	((
          *(img_orig + pix_idx) +
          *(img_orig + pix_idx + 1) +
          *(img_orig + pix_idx + 2)) / 3
        );
    
    /* sauvegarde du resultat */
    for (channel_idx = 0; channel_idx < nb_channels; channel_idx++) {
      *(img_res + pix_idx + channel_idx) = pix_mean;
    }
  }
}

