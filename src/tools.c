#include "tools.h"

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

// Compute distance
double euclidean_distance(unsigned* vec, unsigned cluster_value) {
  // Becuase the clusters coordinates are in the homogenious space,
  // we only need one value to compute their distance
  double dist = 0;
  for (size_t i = 0; i < VECTOR_SIZE; ++i)
    dist += pow(vec[i] - cluster_value, 2);
  return dist;
}

// Sort array
void quick_sort(unsigned *arr, size_t low, size_t high) {
    int q;
    printf("%d, %d, %d\n", arr[low], low, high);
    if (low < high) {
        q = partition(arr, low, high);
        quick_sort(arr, low, q);
        quick_sort(arr, q+1, high);
    }
}

size_t partition(unsigned *arr, size_t low, size_t high) {
    size_t pivot = arr[low], i = low-1, j = high+1;
    while (1) {
        do
            j--;
        while (arr[j] > pivot);
        do
            i++;
        while (arr[i] < pivot);
        if (i < j)
          swap(&arr[i],&arr[j]);
        else
            return j;
    }
}

// Other
void copy_vector(unsigned *src_vector, size_t vector_size, unsigned *res_vector) {
  for (size_t i = 0; i < vector_size; ++i)
    res_vector[i] = src_vector[i];
}

void fill_vector(unsigned value, unsigned *vector) {
  printf("Centroid value = %d\n", value); 
  for (size_t i = 0; i < VECTOR_SIZE; ++i)
    vector[i] = value;
}

unsigned get_pixel_index(unsigned x, unsigned y, size_t width) {
  return x + y * width;
}

void swap(unsigned *a, unsigned *b) {
  unsigned t = *a;
  *a = *b;
  *b = t;
}

void print_array(unsigned *arr, size_t array_size) {
  for (size_t i = 0; i < array_size; ++i)
    printf("%d ", arr[i]);
  printf("\n\n\n");

}
