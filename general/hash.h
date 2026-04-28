#include <stddef.h>

typedef unsigned long hash_t;
typedef const char* hashData_t; // int double

hash_t hash(const void* ptr, size_t size);

hash_t constHash(hashData_t str);
hash_t lenHash(hashData_t str);
hash_t firstSymHash(hashData_t str);
hash_t sumHash(hashData_t str);
hash_t gnuHash(hashData_t str);
hash_t crcHash(hashData_t str);
hash_t rolHash(hashData_t str);
hash_t murMurHash(hashData_t str);

hash_t crcOptimizedHash(hashData_t str);
hash_t crcUnwrapOptimizedHash(hashData_t str);