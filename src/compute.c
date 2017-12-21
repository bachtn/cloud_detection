#include "compute.h"

void compute_image(guchar *img_src, int nb_line, int nb_col, guchar *img_res) {
  Image src_img_struct = { .src = img_src, .width = nb_col, .height = nb_line };
  grayscale(&src_img_struct, img_res);
  Image res_img_struct = { .src = img_res, .width = nb_col, .height = nb_line };
  unsigned* pixel_clusters = malloc(sizeof(unsigned) * (nb_col - 2) *(nb_line - 2));
  kmeans(&res_img_struct, pixel_clusters);
  color_clusters(&res_img_struct, pixel_clusters);
}


void kmeans(struct Image *res_img_struct, unsigned* pixel_clusters) {
  // Returns the cluster of each pixel
  // Init the centroid vector for each cluster
  unsigned **centroid_vectors = malloc(sizeof(unsigned*) * NB_CLUSTERS);
  init_cluster_centroids(NB_CLUSTERS, centroid_vectors);
  // Init the pixel vectors (Vector that contains the pixel with its 4 neighbors)
  int nb_vectors = (res_img_struct->width - 2) * (res_img_struct->height - 2);
  unsigned **pixel_vectors = malloc(sizeof(unsigned*) * nb_vectors);
  get_pixel_vectors(res_img_struct, pixel_vectors);

}


void init_cluster_centroids(unsigned nb_centroids, unsigned** centroid_vectors) {
  for (size_t centroid_idx = 0; centroid_idx < nb_centroids; ++centroid_idx) {
    unsigned *centroid_vector = malloc(sizeof(unsigned) * VECTOR_SIZE);
    fill_vector(255 / (NB_CLUSTERS - 1) * centroid_idx, centroid_vector);
  }
}

void fill_vector(unsigned value, unsigned *vector) {
  printf("Centroid value = %d\n", value); 
  for (size_t i = 0; i < VECTOR_SIZE; ++i)
    vector[i] = value;
}

void get_pixel_vectors(struct Image *img_struct, unsigned **pixel_vectors) {
  // The boundary pixels are not taken into account
  for (size_t y = 1; y < img_struct->width - 1; ++y) {
    for (size_t x = 1; x < img_struct->height - 1; ++x) {
      unsigned pixel_idx = get_pixel_index(x, y, img_struct->width);
      pixel_vectors[pixel_idx] = malloc(sizeof(unsigned) * VECTOR_SIZE);
      get_pixel_vector(img_struct, x, y, pixel_vectors[pixel_idx]);
    }
  }
}

void get_pixel_vector(struct Image *img_struct, unsigned x, unsigned y, unsigned *pixel_vector) {
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

void grayscale(struct Image *img_struct, guchar *img_res) {
  printf("Segmentation de l'image.... A vous!\n");
  //printf("%zu, %zu\n", img_struct->height, img_struct->width);
  for (size_t y = 0; y < img_struct->height; ++y) {
    for (size_t x = 0; x < img_struct->width; ++x) {
      unsigned pixel_idx = get_pixel_index(x, y, img_struct->width) * NB_CHANNELS;
      guchar pix_mean = (unsigned char)	(
          (img_struct->src[pixel_idx] +
           img_struct->src[pixel_idx + 1] + 
           img_struct->src[pixel_idx + 2]) / 3);

      for (unsigned channel_idx = 0; channel_idx < NB_CHANNELS; ++channel_idx)
        img_res[pixel_idx + channel_idx] = pix_mean;
    }
  }
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

unsigned get_pixel_index(unsigned x, unsigned y, size_t width) {
  return x + y * width;
}
