#ifndef _TOOLSH_
#define _TOOLSH_

#include <math.h>
#include <stddef.h>
#include<stdio.h>
#include <gtk/gtk.h>
#include <stdlib.h>

#define VECTOR_SIZE 5
#define NB_CHANNELS 3

typedef struct Image {
  guchar *src;
  size_t width;
  size_t height;
} Image;

// Image
void grayscale(struct Image *img, guchar *img_res);
// Distance
double euclidean_distance(unsigned* vec, unsigned cluster_value);
// Sort
void quick_sort(unsigned *arr, size_t low, size_t high);
size_t partition (unsigned *arr, size_t low, size_t high);
// Other
void copy_vector(unsigned *src_vector, size_t vector_size, unsigned *res_vector);
void fill_vector(unsigned value, unsigned *vector);
unsigned get_pixel_index(unsigned x, unsigned y, size_t width);
void print_array(unsigned *arr, size_t array_size);
void swap(unsigned *a, unsigned *b);

#endif
