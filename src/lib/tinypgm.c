/* -*- mode: c; tab-width: 2; indent-tabs-mode: nil; -*- */

/*******************************************************************************
* tinypgm - A small I/O library for PGM format image files.
*
* Copyright (c) 2016 Marcus Geelnard
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

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*******************************************************************************
* Private functions.
*******************************************************************************/

#define TPGM_MAX_TOKEN_SIZE_ 50

static tpgm_status_t load_next_ascii_token_from_file(FILE* fp, char* token) {
  int is_comment, c, idx;
  do {
    is_comment = 0;
    idx = 0;
    do {
      c = fgetc(fp);
      if (c == EOF) {
        return TPGM_FAIL;
      } else if (c == '#') {
        is_comment = 1;
      }
      if (!is_comment && !isspace(c)) {
        /* Append character to the token string. */
        token[idx++] = (char)c;
        if (idx >= TPGM_MAX_TOKEN_SIZE_) {
          return TPGM_FAIL;
        }
      }
    } while ((!is_comment && !isspace(c)) || (is_comment && c != '\n'));

    /* Zero-terminate the token string. */
    token[idx] = (char)0;
  } while (is_comment);

  return TPGM_OK;
}

static tpgm_status_t load_info_from_file(FILE* fp, tpgm_info_t* info) {
  char token[TPGM_MAX_TOKEN_SIZE_];

  /* The first token must be the string "P5". */
  if (!load_next_ascii_token_from_file(fp, token)) {
    return TPGM_FAIL;
  }
  if (strcmp(token, "P5") != 0) {
    return TPGM_FAIL;
  }

  /* Get the image width. */
  if (!load_next_ascii_token_from_file(fp, token)) {
    return TPGM_FAIL;
  }
  if (sscanf(token, "%d", &info->width) < 1) {
    return TPGM_FAIL;
  }

  /* Get the image height. */
  if (!load_next_ascii_token_from_file(fp, token)) {
    return TPGM_FAIL;
  }
  if (sscanf(token, "%d", &info->height) < 1) {
    return TPGM_FAIL;
  }

  /* Get the maximum pixel value (usually 255). */
  if (!load_next_ascii_token_from_file(fp, token)) {
    return TPGM_FAIL;
  }
  if (sscanf(token, "%d", &info->max_value) < 1) {
    return TPGM_FAIL;
  }

  /* Sanity check of the image dimensions. */
  if (info->width < 1 || info->height < 1 || info->max_value < 1 ||
      info->max_value > 255) {
    return TPGM_FAIL;
  }

  /* Calculate the data size. */
  info->data_size = (size_t)info->width * (size_t)info->height;

  return TPGM_OK;
}

/*******************************************************************************
* Public functions.
*******************************************************************************/

tpgm_status_t tpgm_load_info(const char* file_name, tpgm_info_t* info) {
  FILE* fp;
  tpgm_status_t result;

  /* Check input arguments. */
  if (file_name == NULL || info == NULL) {
    return TPGM_FAIL;
  }

  /* Open the image file. */
  fp = fopen(file_name, "rb");
  if (fp == NULL) {
    return TPGM_FAIL;
  }

  /* Load the header information form the file. */
  result = load_info_from_file(fp, info);

  /* Close the image file. */
  fclose(fp);

  return result;
}

tpgm_status_t tpgm_load_data(const char* file_name,
                             tpgm_info_t* info,
                             void* data,
                             size_t data_size) {
  FILE* fp;
  tpgm_info_t tmp_info;
  size_t bytes_read;

  /* Check input arguments. */
  if (file_name == NULL || data == NULL) {
    return TPGM_FAIL;
  }

  /* Should we return the file information to the caller? */
  if (info == NULL) {
    info = &tmp_info;
  }

  /* Open the image file. */
  fp = fopen(file_name, "rb");
  if (fp == NULL) {
    return TPGM_FAIL;
  }

  /* Load the header information form the file. */
  if (!load_info_from_file(fp, info)) {
    fclose(fp);
    return TPGM_FAIL;
  }

  /* Check that the image data will fit in the data array. */
  if (info->data_size > data_size) {
    fclose(fp);
    return TPGM_FAIL;
  }

  /* Load the data from the file. */
  bytes_read = fread(data, 1, info->data_size, fp);
  if (bytes_read != info->data_size) {
    fclose(fp);
    return TPGM_FAIL;
  }

  /* Close the image file. */
  fclose(fp);

  return TPGM_OK;
}

tpgm_status_t tpgm_save(const char* file_name,
                        const void* data,
                        int width,
                        int height,
                        int stride) {
  FILE* fp;
  const char* data_ptr;
  size_t bytes_written;
  int k;

  /* If the stride is not given, it should be the same as the width. */
  if (stride <= 0) {
    stride = width;
  }

  /* Check input arguments. */
  if (file_name == NULL || data == NULL) {
    return TPGM_FAIL;
  }

  /* Sanity check of the image dimensions. */
  if (width < 1 || height < 1 || stride < width) {
    return TPGM_FAIL;
  }

  /* Open the image file. */
  fp = fopen(file_name, "wb");
  if (fp == NULL) {
    return TPGM_FAIL;
  }

  /* Write the header to the file. */
  fprintf(fp, "P5\n# Created by tinypgm\n%d %d\n%d\n", width, height, 255);

  /* Save the data to the file. */
  data_ptr = (const char*)data;
  for (k = 0; k < height; ++k) {
    bytes_written = fwrite(data_ptr, 1, (size_t)width, fp);
    if (bytes_written != (size_t)width) {
      fclose(fp);
      return TPGM_FAIL;
    }
    data_ptr += stride;
  }

  /* Close the image file. */
  fclose(fp);

  return TPGM_OK;
}
