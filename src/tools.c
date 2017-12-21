#include "tools.h"

double euclidean_distance(unsigned* vec1, unsigned* vec2, int vec_size) {
  double dist = 0;
  for (size_t i = 0; i < vec_size; ++i)
    dist += pow(vec1[i] - vec2[i], 2);
  return dist;
}
