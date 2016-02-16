#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint64_t index_t;
typedef uint64_t ourkey_t;
typedef struct {
  index_t size; // in bits
  index_t count; // in bits
  index_t *table;
} bloom_filter_t;
#define N_HASHES 3

void set_bit(bloom_filter_t *B, index_t i);//done
index_t get_bit(bloom_filter_t *B, index_t i);//done

index_t hash1(bloom_filter_t *B, ourkey_t k);
index_t hash2(bloom_filter_t *B, ourkey_t k);

void bloom_init(bloom_filter_t *B, index_t size_in_bits);//done
void bloom_destroy(bloom_filter_t *B);//done
int bloom_check(bloom_filter_t *B, ourkey_t k);
void bloom_add(bloom_filter_t *B, ourkey_t k);
