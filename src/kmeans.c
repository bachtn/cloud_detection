#include "kmeans.h"

void kmeans(struct Image *res_img_struct, unsigned* pixel_clusters) {
  /* Returns the cluster of each pixel
   * Init the centroid coordinate of each cluster
   * Each centroid is caracterised by a single coordinate because
   * they are projected on the homogenious axes (All vector
   * coordinates are the same)
   */
  unsigned *centroids = malloc(sizeof(unsigned) * NB_CLUSTERS);
  init_cluster_centroids(NB_CLUSTERS, centroids);
  //quick_sort(centroids, 0, NB_CLUSTERS - 1);
  //print_array(centroids, NB_CLUSTERS);

  // Init the pixel vectors (Vector of the pixel with its 4 neighbors)
  int nb_vectors = (res_img_struct->width - 2) * (res_img_struct->height - 2);
  unsigned **pixel_vectors = malloc(sizeof(unsigned*) * nb_vectors);
  get_pixel_vectors(res_img_struct, pixel_vectors);

  unsigned nb_iterations = 0;
  unsigned *old_centroids = (unsigned *) calloc(NB_CLUSTERS, sizeof(unsigned));

  do {
    // Conserve the old clusters (needed for convergence check)
    copy_vector(centroids, NB_CLUSTERS, old_centroids);
    // Assign new clusters to the pixels depending on the new clusters
    // coordinates
    assign_cluster(pixel_vectors, centroids, pixel_clusters);
    // Update the cluster coordinates depending on the new pixel clusters
    update_cluster_centroids(pixel_vectors, pixel_clusters, centroids);
  } while (++nb_iterations < MAX_ITERATIONS || has_converged(centroids, old_centroids));
}


void assign_cluster(unsigned **pixel_vectors, unsigned *centroids,
    /* Computes the distance between each pixel vector and each cluster centroid
     * Assign each pixel to the nearest cluster
     */
    unsigned *pixel_clusters) {
  //FIXME
   
}

void update_cluster_centroids(unsigned **pixel_vectors, unsigned *pixel_clusters,
    // Computes the new coordinates of each cluster centroid
    unsigned *centroids) {
  //FIXME
}


void init_cluster_centroids(unsigned nb_centroids, unsigned* centroids) {
  // Init cluster centroids to 0 63 126 189 252 if NB_CLUSTERS = 5
  for (size_t centroid_idx = 0; centroid_idx < nb_centroids; ++centroid_idx)
    centroids[centroid_idx] = 255 / (NB_CLUSTERS - 1) * centroid_idx;
}

// We have convergence only if the centroid coordinates no longer or slighly
// change
int has_converged(unsigned *current_centroid_vectors, unsigned *old_centroid_vectors) {
  for (size_t i = 0; i < NB_CLUSTERS; ++i)
    if (abs(current_centroid_vectors[i] - old_centroid_vectors[i]) > CONVERGENCE_THRESHOLD)
        return 1;
  return 0;
}

void get_pixel_vectors(struct Image *img_struct,
    unsigned **pixel_vectors) {
  /* Returns each pixel with its 4 neighbors (up,right,bottom,left)
   * The boundary pixels are not taken into account
   */
  for (size_t y = 1; y < img_struct->width - 1; ++y) {
    for (size_t x = 1; x < img_struct->height - 1; ++x) {
      unsigned pixel_idx = get_pixel_index(x, y, img_struct->width);
      pixel_vectors[pixel_idx] = malloc(sizeof(unsigned) * VECTOR_SIZE);
      get_pixel_vector(img_struct, x, y, pixel_vectors[pixel_idx]);
    }
  }
}

void get_pixel_vector(struct Image *img_struct, unsigned x,
    unsigned y, unsigned *pixel_vector) {
  // Return a vector with the pixel and its 4 neighbors
  pixel_vector[0] = img_struct->src[get_pixel_index(x, y-1,
      img_struct->width) * NB_CHANNELS]; // up
  pixel_vector[1] = img_struct->src[get_pixel_index(x-1, y,
      img_struct->width) * NB_CHANNELS]; // left
  pixel_vector[2] = img_struct->src[get_pixel_index(x, y,
      img_struct->width) * NB_CHANNELS];   // current
  pixel_vector[3] = img_struct->src[get_pixel_index(x+1, y,
      img_struct->width) * NB_CHANNELS]; // right
  pixel_vector[4] = img_struct->src[get_pixel_index(x, y+1,
      img_struct->width) * NB_CHANNELS]; // down
}

void color_clusters(struct Image *res_img_struct, unsigned *pixel_clusters) {
  for (size_t y = 1; y < res_img_struct->height - 1; ++y) {
    for (size_t x = 1; x < res_img_struct->width - 1; ++x) {
      unsigned pixel_idx = get_pixel_index(x, y, res_img_struct->width) * NB_CHANNELS;
      unsigned idx = (y - 1) * (res_img_struct->width - 2) + (x - 1); //TODO: Test
      if (pixel_clusters[idx] > NB_CLUSTERS * CLOUD_PERCENT) {
        res_img_struct->src[pixel_idx] = 255;
        res_img_struct->src[pixel_idx + 1] = 0;
        res_img_struct->src[pixel_idx + 2] = 0;
      }
    }
  }
}
