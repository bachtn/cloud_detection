#ifndef _COMPUTEH_
#define _COMPUTEH_

#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include "tools.h"
#include "kmeans.h"

#define LargeurImage 360
void compute_image(guchar *img_src, int nb_line, int nb_col, guchar *img_res);

#endif
