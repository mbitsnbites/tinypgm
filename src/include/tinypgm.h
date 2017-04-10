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

#ifndef TPGM_TINYPGM_H
#define TPGM_TINYPGM_H

/**
* @file tinypgm.h
* @brief Interface declaration for the tinypgm library.
*
* @mainpage
* See tinypgm.h for the full API documentation.
*/

#include <stddef.h> /* For size_t */

#ifdef __cplusplus
extern "C" {
#endif

/**
* @brief Return value for all tinypgm library functions.
*/
typedef enum {
  TPGM_FAIL = 0, /**< Failure (zero). */
  TPGM_OK = 1    /**< Success (non-zero). */
} tpgm_status_t;

/**
* @brief Image information structure.
*/
typedef struct {
  int width;        /**< Image width (pixels). */
  int height;       /**< Image height (pixels). */
  int max_value;    /**< Maximum pixel value (usually 255). */
  size_t data_size; /**< Size of the image data, in bytes. */
} tpgm_info_t;

/**
* @brief Load image information from a file.
* @param file_name Name of the file to load (UTF-8).
* @param[out] info Returned image information.
* @returns TPGM_OK on success, else TPGM_FAIL.
*/
tpgm_status_t tpgm_load_info(const char* file_name, tpgm_info_t* info);

/**
* @brief Load image information and data from a file.
* @param file_name Name of the file to load (UTF-8).
* @param[out] info Returned image information (may be NULL).
* @param[out] data A byte array that will be filled with the image data.
* @param data_size Size of the data array, in bytes. If the image data does not
* fit within this limit, the function will abort with the status code TPGM_FAIL.
* @returns TPGM_OK on success, else TPGM_FAIL.
*/
tpgm_status_t tpgm_load_data(const char* file_name,
                             tpgm_info_t* info,
                             void* data,
                             size_t data_size);

/**
* @brief Save image data to a file.
* @param file_name Name of the file to save (UTF-8).
* @param data A byte array holding the image data. The size of the data array
* must be stride * height bytes.
* @param width Image width.
* @param height Image height.
* @param stride Number of bytes per row (including padding). If zero (0), the
* @c width argument will be used as stride (assuming that rows are tightly
* packed in memory).
* @returns TPGM_OK on success, else TPGM_FAIL.
*/
tpgm_status_t tpgm_save(const char* file_name,
                        const void* data,
                        int width,
                        int height,
                        int stride);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* TPGM_TINYPGM_H */
