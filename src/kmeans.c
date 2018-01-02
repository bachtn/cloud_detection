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

  // Init the pixel vectors (Vector of the pixel with its 4 neighbors)
  int nb_vectors = (res_img_struct->width - 2) * (res_img_struct->height - 2);
  unsigned **pixel_vectors = malloc(sizeof(unsigned*) * nb_vectors);
  printf("16 - %d\n", pixel_vectors[248064]);
  get_pixel_vectors(res_img_struct, pixel_vectors);
  printf("18 - %d\n", pixel_vectors[248064]);
  unsigned iteration_idx = 0;
  unsigned *old_centroids = calloc(NB_CLUSTERS, sizeof(unsigned));

  unsigned *cluster_occurences = calloc(NB_CLUSTERS, sizeof(unsigned));

  do {
    // Conserve the old clusters (needed for convergence check)
    copy_vector(centroids, NB_CLUSTERS, old_centroids);
    // Assign new clusters to the pixels depending on the new clusters
    // coordinates
    assign_cluster(res_img_struct, pixel_vectors, centroids, cluster_occurences, pixel_clusters);
    printf("30 - %d\n", pixel_vectors[248064]);
    // Update the cluster coordinates depending on the new pixel clusters
    update_cluster_centroids(nb_vectors, pixel_vectors, cluster_occurences, pixel_clusters, centroids);
    printf("33 - %d\n", pixel_vectors[248064]);
    // Set cluster occurences to 0
    fill_vector(0, NB_CLUSTERS, cluster_occurences);
  } while (++iteration_idx < MAX_ITERATIONS || has_converged(centroids, old_centroids));
  printf("iter = %d\n", iteration_idx);
}

void assign_cluster(struct Image *img_struct, unsigned **pixel_vectors, unsigned *centroids,
    unsigned *cluster_occurences, unsigned *pixel_clusters) {
  printf("--> Assigning pixel clusters\n");
    /* Computes the distance between each pixel vector and each cluster centroid
     * Assign each pixel to the nearest cluster
     */
  // Set cluster_occurences to 0 -> used calloc, normally do not need this
  //for (size_t centroid_idx; centroid_idx < NB_CLUSTERS; ++centroid_idx)
  //  cluster_occurences[centroid_idx] = 0;
  for (size_t y = 1; y < img_struct->width - 1; ++y) {
    for (size_t x = 1; x < img_struct->height - 1; ++x) {
      unsigned pixel_idx = get_pixel_index(x - 1, y - 1, img_struct->width - 2);
      unsigned new_cluster = get_pixel_closest_cluster(pixel_vectors[pixel_idx], centroids);
      unsigned idx = (y - 1) * (img_struct->width - 2) + (x - 1); //TODO: Test
      idx = get_pixel_index(x-1, y-1, img_struct->width - 2); // TODO: choose one
      pixel_clusters[idx] = new_cluster;
      cluster_occurences[new_cluster] += 1;  
    }
  }    
}

void update_cluster_centroids(unsigned nb_pixels, unsigned **pixel_vectors,
    unsigned *cluster_occurences, unsigned *pixel_clusters, unsigned *centroids) {
    printf("--> Updating Cluster centroids\n");
    // Computes the new coordinates of each cluster centroid
    // For each cluster construct a vector with the values of each coordinates
    for (size_t centroid_idx = 0; centroid_idx < NB_CLUSTERS; ++centroid_idx) {
      if (cluster_occurences[centroid_idx] > 0) {
        unsigned *current_centroid = malloc(sizeof(unsigned) * VECTOR_SIZE);
        unsigned new_coordinate = 0;
        for (size_t vec_idx = 0; vec_idx < VECTOR_SIZE; ++vec_idx) {
          // Get coordinate_median
          unsigned median = get_cluster_coordinate_median(centroid_idx, vec_idx, nb_pixels,
            pixel_vectors, cluster_occurences[centroid_idx], pixel_clusters);
          current_centroid[vec_idx] = median;
          new_coordinate += median;
        }
        // Project in the homogenious space
        centroids[centroid_idx] = (unsigned)(new_coordinate / NB_CLUSTERS);
      }
    }
}

//printf("i = %d, pix_idx = %d, cluster_occurence = %d, vec_idx = %d\n",
//    i, pix_idx, cluster_occurence, vec_idx);
unsigned get_cluster_coordinate_median(unsigned centroid_idx, unsigned vec_idx, unsigned nb_pixels,
    unsigned **pixel_vectors, unsigned cluster_occurence, unsigned *pixel_clusters) {
  //printf("--> Computing cluster median\n");
  unsigned *pixel_coordinates = malloc(sizeof(unsigned) * cluster_occurence);
  unsigned pix_idx = 0;
  for (size_t i = 0; i < nb_pixels; ++i) {
    if (pixel_clusters[i] == centroid_idx) {
      pixel_coordinates[pix_idx] = pixel_vectors[i][vec_idx];
      pix_idx += 1;
    }
    if (pix_idx == cluster_occurence) { //TODO check
      break;
    }
    //printf("i = %d, pix_idx = %d, cluster occurence = %d\n", i, pix_idx, cluster_occurence);
  }
  quick_sort(pixel_coordinates, 0, cluster_occurence - 1);
  unsigned median_idx = (unsigned)(cluster_occurence / 2);
  return pixel_coordinates[median_idx];
}

unsigned get_pixel_closest_cluster(unsigned *pixel_vector, unsigned *centroids) {
  //printf("--> Computing pixel closest cluster\n");
  // Compute distance from each cluster
  double min_dist = DBL_MAX; //TODO: Chnage to INT_MAX ?
  int new_cluster_idx = 0;
  for (size_t centroid_idx = 0; centroid_idx < NB_CLUSTERS; ++centroid_idx) {
    unsigned dist = euclidean_distance(pixel_vector, centroids[centroid_idx]);
    if (dist < min_dist) {
      min_dist = dist;
      new_cluster_idx = centroid_idx;
    }
  }
  //printf("dist = %d, cluster = %d", min_dist, new_cluster_idx);
  return new_cluster_idx;
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

void get_pixel_vectors(struct Image *img_struct, unsigned **pixel_vectors) {
  /* Returns each pixel with its 4 neighbors (up,right,bottom,left)
   * The boundary pixels are not taken into account
   */
  for (size_t y = 1; y < img_struct->width - 1; ++y) {
    for (size_t x = 1; x < img_struct->height - 1; ++x) {
      unsigned pixel_idx = get_pixel_index(x - 1, y - 1, img_struct->width - 2);
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
  printf("Coloring\n");
  for (size_t y = 1; y < res_img_struct->height - 1; ++y) {
    for (size_t x = 1; x < res_img_struct->width - 1; ++x) {
      unsigned pixel_idx = get_pixel_index(x, y, res_img_struct->width) * NB_CHANNELS;
      unsigned idx = (y - 1) * (res_img_struct->width - 2) + (x - 1); //TODO: Test
      if (pixel_clusters[idx] > NB_CLUSTERS * CLOUD_PERCENT) { //TODO: check condition
        res_img_struct->src[pixel_idx] = 255;
        res_img_struct->src[pixel_idx + 1] = 0;
        res_img_struct->src[pixel_idx + 2] = 0;
      }
    }
  }
}
