// SOURCE: https://www.cryptologie.net/article/419/zeroing-memory-compiler-optimizations-and-memset_s/
#include "erase_from_memory.h"
#include <stdlib.h>
#include <string.h>

void *erase_from_memory(void *pointer, size_t size_data, size_t size_to_remove) {
  #ifdef __STDC_LIB_EXT1__
   memset_s(pointer, size_data, 0, size_to_remove);
  #else
   if(size_to_remove > size_data) size_to_remove = size_data;
	 volatile unsigned char *p = pointer;
	 while (size_to_remove--){
		 *p++ = 0;
	 }
  #endif
	return pointer;
}
