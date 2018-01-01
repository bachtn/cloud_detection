#include "compute.h"

void compute_image(guchar *img_src, int nb_line, int nb_col,
    guchar *img_res) {
  Image src_img_struct = {
    .src = img_src, .width = nb_col, .height = nb_line
  };
  // Grayscale
  grayscale(&src_img_struct, img_res);
  Image res_img_struct = {
    .src = img_res, .width = nb_col, .height = nb_line
  };
  // Init Pixel clusrters
  unsigned *pixel_clusters = (unsigned *) calloc((nb_col - 2) *
      (nb_line - 2), sizeof(unsigned));
  // Assign clusters with kmeans algorithm
  kmeans(&res_img_struct, pixel_clusters);
  // Color clusters
  // The clusters will be colored in the res_image
  color_clusters(&res_img_struct, pixel_clusters);
}
