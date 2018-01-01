#ifndef _KMEANSH_
#define _KMEANSH_

#include "tools.h"

#define NB_CLUSTERS 5
#define CONVERGENCE_THRESHOLD .0001
#define MAX_ITERATIONS 1000 // max terations by kmeans algorithm
#define CLOUD_PERCENT .7

void kmeans(struct Image *res_img_struct, unsigned* pixel_clusters);
void assign_cluster(unsigned **pixel_vectors, unsigned *centroids,
    unsigned *pixel_clusters);
void update_cluster_centroids(unsigned **pixel_vectors, unsigned *pixel_clusters,
    unsigned *centroids);
void get_pixel_vectors(struct Image *img_struct, unsigned **pixel_vectors);
void get_pixel_vector(struct Image *img_struct, unsigned x, unsigned y, unsigned *pixel_vector);
void init_cluster_centroids(unsigned nb_centroids, unsigned* centroids);
void color_clusters(struct Image *res_img_struct, unsigned *pixel_clusters);
int has_converged(unsigned *current_centroid_vectors, unsigned *old_centroid_vectors);

#endif
