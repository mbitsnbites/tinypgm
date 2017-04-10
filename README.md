# tinypgm

## About

This is a small C library for loading and saving 8-bit PGM
([portable graymap](http://netpbm.sourceforge.net/doc/pgm.html)) images.

## Building

Use CMake to build tinypgm, preferrably out-of-tree. E.g.

```bash
cd my/build/dir
cmake -G Ninja -DCMAKE_INSTALL_PREFIX=path/to/my/install path/to/tinypgm
ninja install
```

## Usage

### Loading an image

Here is an example of loading an image, including all the necessary error handling:

```c
#include <stdio.h>
#include <stdlib.h>
#include <tinypgm.h>

#define MY_PGM_FILE "hello.pgm"

int main() {
  tpgm_info_t info;
  char* data;

  /* Get image information. */
  if (!tpgm_load_info(MY_PGM_FILE, &info)) {
    fprintf(stderr, "Failed to load image.\n");
    return 1;
  }
  printf("Image dimensions: %dx%d (max value: %d)\n",
      info.width, info.height, info.max_value);

  /* Allocate memory for the image data. */
  data = (char*)malloc(info.data_size);
  if (!data) {
    fprintf(stderr, "Failed to allocate memory for the image data.\n");
    return 1;
  }

  /* Load the image data. */
  if (!tpgm_load_data(MY_PGM_FILE, NULL, data, info.data_size)) {
    fprintf(stderr, "Failed to load the image data.\n");
    free(data);
    return 1;
  }

  /* Do something with the image data. */
  /* ... */

  /* Free the image data. */
  free(data);

  return 0;
}
```

### Saving an image

Here is an example of saving an image, including all the necessary error handling:

```c
#include <stdio.h>
#include <tinypgm.h>

#define MY_PGM_FILE "hello.pgm"
#define IMAGE_WIDTH 1280
#define IMAGE_HEIGHT 768

int main() {
  tpgm_info_t info;
  char* data;

  /* Allocate memory for the image. */
  data = (char*)malloc(IMAGE_WIDTH * IMAGE_HEIGHT);
  if (!data) {
    fprintf(stderr, "Failed to allocate memory for the image.\n");
    return 1;
  }

  /* Fill out the image data somehow. */
  /* ... */

  /* Save the image data. */
  if (!tpgm_save_data(MY_PGM_FILE, data, IMAGE_WIDTH, IMAGE_HEIGHT, 0)) {
    fprintf(stderr, "Failed to save the image data.\n");
    free(data);
    return 1;
  }

  /* Free the image data. */
  free(data);

  return 0;
}
```

## License

The library is released under the zlib/libpng license. See [COPYING](COPYING).

