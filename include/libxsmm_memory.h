/******************************************************************************
* Copyright (c) Intel Corporation - All rights reserved.                      *
* This file is part of the LIBXSMM library.                                   *
*                                                                             *
* For information on the license, see the LICENSE file.                       *
* Further information: https://github.com/libxsmm/libxsmm/                    *
* SPDX-License-Identifier: BSD-3-Clause                                       *
******************************************************************************/
/* Hans Pabst (Intel Corp.)
******************************************************************************/
#ifndef LIBXSMM_MEMORY_H
#define LIBXSMM_MEMORY_H

#include "libxsmm_macros.h"

#define LIBXSMM_MEMORY127_LOOP(DST, SRC, SIZE, RHS, NTS) do { \
  const signed char libxsmm_memory127_loop_size_ = LIBXSMM_CAST_ICHAR(SIZE); \
  unsigned char *const LIBXSMM_RESTRICT libxsmm_memory127_loop_dst_ = (unsigned char*)(DST); \
  signed char libxsmm_memory127_loop_i_; \
  NTS(libxsmm_memory127_loop_dst_) LIBXSMM_PRAGMA_UNROLL \
  for (libxsmm_memory127_loop_i_ = 0; libxsmm_memory127_loop_i_ < libxsmm_memory127_loop_size_; \
    ++libxsmm_memory127_loop_i_) \
  { \
    RHS(unsigned char, libxsmm_memory127_loop_dst_, SRC, libxsmm_memory127_loop_i_); \
  } \
} while(0)
#define LIBXSMM_MEMORY127_NTS(...)

#define LIBXSMM_MEMSET127_RHS(TYPE, DST, SRC, IDX) \
  ((DST)[IDX] = (TYPE)(SRC))
#define LIBXSMM_MEMSET127(DST, SRC, SIZE) \
  LIBXSMM_MEMORY127_LOOP(DST, SRC, SIZE, \
  LIBXSMM_MEMSET127_RHS, LIBXSMM_MEMORY127_NTS)
#define LIBXSMM_MEMZERO127(DST) LIBXSMM_MEMSET127(DST, 0, sizeof(*(DST)))

#define LIBXSMM_MEMCPY127_RHS(TYPE, DST, SRC, IDX) \
  ((DST)[IDX] = ((const TYPE*)(SRC))[IDX])
#define LIBXSMM_MEMCPY127(DST, SRC, SIZE) \
  LIBXSMM_MEMORY127_LOOP(DST, SRC, SIZE, \
  LIBXSMM_MEMCPY127_RHS, LIBXSMM_MEMORY127_NTS)
#define LIBXSMM_ASSIGN127(DST, SRC) do { \
  LIBXSMM_ASSERT(sizeof(*(SRC)) <= sizeof(*(DST))); \
  LIBXSMM_MEMCPY127(DST, SRC, sizeof(*(SRC))); \
} while(0)

#define LIBXSMM_MEMSWP127_RHS(TYPE, DST, SRC, IDX) \
  LIBXSMM_ISWAP((DST)[IDX], ((TYPE*)(SRC))[IDX])
#define LIBXSMM_MEMSWP127(DST, SRC, SIZE) \
  LIBXSMM_MEMORY127_LOOP(DST, SRC, SIZE, \
  LIBXSMM_MEMSWP127_RHS, LIBXSMM_MEMORY127_NTS)


/**
 * Check if pointer is SIMD-aligned and optionally consider the next access (increment in Bytes).
 * Optionally calculates the alignment of the given pointer in Bytes.
 */
LIBXSMM_API int libxsmm_aligned(const void* ptr, const size_t* inc, int* alignment);

/**
 * Calculates if there is a difference between two (short) buffers.
 * Returns zero if there is no difference; otherwise non-zero.
 */
LIBXSMM_API unsigned char libxsmm_diff(const void* a, const void* b, unsigned char size);

/**
 * Calculates the "difference" between "a" and "b"; "a" is taken "count" times into account.
 * Returns the first match (index) of no difference (or "n" if "a" did not match).
 * The hint determines the initial index searching for a difference, and it must
 * be in bounds [0, count), but otherwise only impacts performance.
 */
LIBXSMM_API unsigned int libxsmm_diff_n(const void* a, const void* bn, unsigned char elemsize,
  unsigned char stride, unsigned int hint, unsigned int count);

/** Similar to memcmp (C standard library), but the result is conceptually only a boolean. */
LIBXSMM_API int libxsmm_memcmp(const void* a, const void* b, size_t size);

/** Calculate a hash value for the given buffer and seed; accepts NULL-buffer. */
LIBXSMM_API unsigned int libxsmm_hash(const void* data, unsigned int size, unsigned int seed);

/** Calculate a 64-bit hash for the given character string; accepts NULL-string. */
LIBXSMM_API unsigned long long libxsmm_hash_string(const char string[]);

/** Return the pointer to the 1st match of "b" in "a", or NULL (no match). */
LIBXSMM_API const char* libxsmm_stristr(const char a[], const char b[]);

/** Determines the number of calls to restore the original data (libxsmm_shuffle). */
LIBXSMM_API size_t libxsmm_unshuffle(size_t count,
  /* Shall be coprime to count (libxsmm_coprime2(count) if NULL). */
  const size_t* shuffle);

/** Out-of-place shuffling of data given by elemsize and count. */
LIBXSMM_API void libxsmm_shuffle(void* dst, const void* src, size_t elemsize, size_t count,
  /* Shall be coprime to count (libxsmm_coprime2(count) if NULL). */
  const size_t* shuffle,
  /* If NULL, the default value is one. */
  const size_t* nrepeat);

#endif /*LIBXSMM_MEMORY_H*/
