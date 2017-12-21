#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include "tools.h"

#define LargeurImage 360
#define VECTOR_SIZE 5
#define NB_CLUSTERS 5
#define NB_CHANNELS 3
#define CLOUD_PERCENT .7


typedef struct Image {
  guchar *src;
  size_t width;
  size_t height;
} Image;

void compute_image(guchar *img_src, int nb_line, int nb_col, guchar *img_res);
void grayscale(struct Image *img, guchar *img_res);
void kmeans(struct Image *res_img_struct, unsigned* pixel_clusters);
void get_pixel_vectors(struct Image *img_struct, unsigned **pixel_vectors);
void get_pixel_vector(struct Image *img_struct, unsigned x, unsigned y, unsigned *pixel_vector);
void init_cluster_centroids(unsigned nb_centroids, unsigned** centroid_vectors);
void color_clusters(struct Image *res_img_struct, unsigned *pixel_clusters);
unsigned get_pixel_index(unsigned x, unsigned y, size_t width);
void fill_vector(unsigned value, unsigned *vector);
