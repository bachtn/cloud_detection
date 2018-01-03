#ifndef _KMEANSH_
#define _KMEANSH_

#include "tools.h"

#define NB_CLUSTERS 5
#define CLOUD_PERCENT 0.5f
#define CONVERGENCE_THRESHOLD 0.001f
#define MAX_ITERATIONS 25 // max terations by kmeans algorithm
// 5 and .5 good

void kmeans(struct Image *res_img_struct, unsigned* pixel_clusters);
void assign_cluster(struct Image *img_struct, unsigned **pixel_vectors, unsigned *centroids,
    unsigned *cluster_occurences, unsigned *pixel_clusters);
void update_cluster_centroids(unsigned nb_pixels, unsigned **pixel_vectors,
    unsigned *cluster_occurences, unsigned *pixel_clusters, unsigned *centroids);
unsigned get_cluster_coordinate_median(unsigned centroid_idx, unsigned vec_idx, unsigned nb_pixels,
    unsigned **pixel_vectors, unsigned cluster_occurence, unsigned *pixel_clusters);
unsigned get_pixel_closest_cluster(unsigned *pixel_vector, unsigned *centroids);
void get_pixel_vectors(struct Image *img_struct, unsigned **pixel_vectors);
void get_pixel_vector(struct Image *img_struct, unsigned x, unsigned y, unsigned *pixel_vector);
void init_cluster_centroids(unsigned nb_centroids, unsigned* centroids);
float color_clusters(struct Image *res_img_struct, unsigned *pixel_clusters);
int has_converged(unsigned *current_centroid_vectors, unsigned *old_centroid_vectors);

#endif
