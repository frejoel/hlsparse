#ifndef _MEMORY_H
#define _MEMORY_H

#include "hlsparse.h"

#ifdef MEM_EXTERN_ALLOC
#   define EXTERN extern
#else
#   define EXTERN
#endif

// #ifdef __cplusplus
// extern "C" {
// #endif

// Memory Allocator
EXTERN hlsparse_malloc_callback hls_malloc;
EXTERN hlsparse_free_callback hls_free;

// #ifdef __cplusplus
// }
// #endif

#endif // _MEMORY_H