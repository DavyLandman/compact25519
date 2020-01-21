#ifndef __ERASE_FROM_MEMORY_H__
#define __ERASE_FROM_MEMORY_H__

// SOURCE: https://www.cryptologie.net/article/419/zeroing-memory-compiler-optimizations-and-memset_s/

#define __STDC_WANT_LIB_EXT1__ 1
#include <stdint.h>
#include <stddef.h>

void *erase_from_memory(void *pointer, size_t size_data, size_t size_to_remove); 

#endif
