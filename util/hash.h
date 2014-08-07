#ifndef _UTIL_HASH_H_INCLUDED__
#define _UTIL_HASH_H_INCLUDED__

#ifdef __cplusplus
extern "c" {
#endif

//copy from memcached 1.4.15
#include <stdint.h>
uint32_t hash(const void *key, size_t length, const uint32_t initval);

#ifdef __cplusplus
}
#endif

#endif

