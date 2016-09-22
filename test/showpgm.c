#include <stdio.h>
#include <stdlib.h>
#include <tinypgm.h>

/* Convert a pixel value (0-255) to an ASCII character. */
static char pixToChar(int p) {
  static const char* lookup = " .-~=:;+<coaq08@";
  p = (p + 8) / 16;
  return lookup[p < 0 ? 0 : p > 15 ? 15 : p];
}

int main(int argc, char** argv) {
  tpgm_info_t info;
  char* file_name;
  unsigned char* data;
  int h_scale = 12, v_scale;
  int x, y, xx, yy, xmax, ymax, pix, count;

  /* Check input arguments. */
  if (argc < 2) {
    printf("Usage: %s image [scale]\n", argv[0]);
    printf("  image - The file to display\n");
    printf("  scale - The image scale (default: 12)\n");
    return 1;
  }
  file_name = argv[1];
  if (argc >= 3) {
    if (sscanf(argv[2], "%d", &x) && x >= 1 && x <= 1000) {
      h_scale = x;
    }
  }

  /* Get image information. */
  if (!tpgm_load_info(file_name, &info)) {
    fprintf(stderr, "Failed to load image.\n");
    return 1;
  }
  printf("Image dimensions: %dx%d (max value: %d)\n", info.width, info.height,
         info.max_value);

  /* Allocate memory for the image data. */
  data = (unsigned char*)malloc(info.data_size);
  if (!data) {
    fprintf(stderr, "Failed to allocate memory for the image data.\n");
    return 1;
  }

  /* Load the image data. */
  if (!tpgm_load_data(file_name, NULL, data, info.data_size)) {
    fprintf(stderr, "Failed to load the image data.\n");
    free(data);
    return 1;
  }

  /* Print the image to stdout as ASCII. */
  v_scale = (h_scale * 22 + 11) / 12;
  for (y = 0; y < info.height;) {
    ymax = y + v_scale;
    if (ymax > info.height)
      ymax = info.height;
    for (x = 0; x < info.width;) {
      xmax = x + h_scale;
      if (xmax > info.width)
        xmax = info.width;

      /* Average several pixels into one (scale down). */
      pix = 0;
      count = 0;
      for (yy = y; yy < ymax; ++yy) {
        for (xx = x; xx < xmax; ++xx) {
          pix += (int)data[yy * info.width + xx];
          count++;
        }
      }
      putchar(pixToChar(count ? (pix + count / 2) / count : 0));

      x = xmax;
    }
    y = ymax;
    printf("\n");
  }

  /* Free the image data. */
  free(data);

  return 0;
}
