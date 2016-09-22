/* -*- mode: c; tab-width: 2; indent-tabs-mode: nil; -*- */

/*******************************************************************************
* tinypgm - A small I/O library for PGM format image files.
*
* Copyright (c) 2016 Smart Eye AB
*
* This software is provided 'as-is', without any express or implied warranty. In
* no event will the authors be held liable for any damages arising from the use
* of this software.
*
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it freely,
* subject to the following restrictions:
*
*     1. The origin of this software must not be misrepresented; you must not
*        claim that you wrote the original software. If you use this software in
*        a product, an acknowledgment in the product documentation would be
*        appreciated but is not required.
*
*     2. Altered source versions must be plainly marked as such, and must not be
*        misrepresented as being the original software.
*
*     3. This notice may not be removed or altered from any source distribution.
*
*******************************************************************************/

#include <tinypgm.h>

#include <stdio.h>
#include <stdlib.h>

#define TEST_BUFFER_SIZE 100
static char test_buffer[TEST_BUFFER_SIZE];

int main(int argc, char** argv) {
  tpgm_info_t info;
  const char *good_file, *bad_file;
  char* data;
  size_t data_size;

  /* Check input arguments. */
  if (argc != 3) {
    printf("Usage: %s good-file bad-file\n", argv[0]);
    return 1;
  }
  good_file = argv[1];
  bad_file = argv[2];

  /* Load info from a non-existing file -> TPGM_FAIL */
  printf("Test 1: ");
  if (tpgm_load_info("_NOSUCHFILE_.pgm", &info) == TPGM_FAIL) {
    printf("PASSED\n");
  } else {
    printf("FAILED\n");
  }

  /* Load info from a bad PGM file -> TPGM_FAIL */
  printf("Test 2: ");
  if (tpgm_load_info(bad_file, &info) == TPGM_FAIL) {
    printf("PASSED\n");
  } else {
    printf("FAILED\n");
  }

  /* Load info from a proper PGM file -> TPGM_OK */
  printf("Test 3: ");
  if (tpgm_load_info(good_file, &info) == TPGM_OK) {
    printf("PASSED\n");
  } else {
    printf("FAILED\n");
  }

  /* Load info from a proper PGM file, passing NULL as info -> TPGM_FAIL */
  printf("Test 4: ");
  if (tpgm_load_info(good_file, NULL) == TPGM_FAIL) {
    printf("PASSED\n");
  } else {
    printf("FAILED\n");
  }

  /* Load data from a non-existing file -> TPGM_FAIL */
  printf("Test 5: ");
  data = test_buffer;
  if (tpgm_load_data("_NOSUCHFILE_.pgm", &info, data, TEST_BUFFER_SIZE) ==
      TPGM_FAIL) {
    printf("PASSED\n");
  } else {
    printf("FAILED\n");
  }

  /* Load data from a bad PGM file -> TPGM_FAIL */
  printf("Test 6: ");
  if (tpgm_load_data(bad_file, &info, data, 0) == TPGM_FAIL) {
    printf("PASSED\n");
  } else {
    printf("FAILED\n");
  }

  if (tpgm_load_info(good_file, &info) == TPGM_OK) {
    printf("\nTesting with \"%s\", %dx%d (max value: %d, data size: %ld)\n",
           good_file, info.width, info.height, info.max_value, info.data_size);

    /* Allocate memory for the data. */
    data_size = info.data_size;
    data = (char*)malloc(data_size);

    /* Load data from a proper PGM file -> TPGM_OK */
    printf("  Test 7: ");
    if (tpgm_load_data(good_file, &info, data, data_size) == TPGM_OK) {
      printf("PASSED\n");
    } else {
      printf("FAILED\n");
    }

    /* Load data from a proper PGM file, passing NULL as info -> TPGM_OK */
    printf("  Test 8: ");
    if (tpgm_load_data(good_file, NULL, data, data_size) == TPGM_OK) {
      printf("PASSED\n");
    } else {
      printf("FAILED\n");
    }

    /* Load data from a proper PGM file, passing NULL as data -> TPGM_FAIL */
    printf("  Test 9: ");
    if (tpgm_load_data(good_file, NULL, NULL, data_size) == TPGM_FAIL) {
      printf("PASSED\n");
    } else {
      printf("FAILED\n");
    }

    /* Load data from a proper PGM file, passing too small size -> TPGM_FAIL */
    printf("  Test 10: ");
    if (tpgm_load_data(good_file, NULL, data, data_size / 2) == TPGM_FAIL) {
      printf("PASSED\n");
    } else {
      printf("FAILED\n");
    }

    free(data);
  } else {
    printf("\nFailed to load image header for file \"%s\".\n", good_file);
    printf("Unable to perform image loading tests.\n");
  }

  return 0;
}
