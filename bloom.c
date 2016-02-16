#include <stdint.h>
#include <stdio.h>
#include "bloom.h"
#include <inttypes.h>
void bloom_init(bloom_filter_t *B, index_t size_in_bits)
{
    B->table = (uint64_t *) calloc(size_in_bits/sizeof(index_t)+1,sizeof(index_t));
	index_t key =0; 
    if (B->table == NULL) {
		perror("malloc failed");
		free(B->table);
    }
	B->count = 0;
    B->size = size_in_bits;
}
void bloom_destroy(bloom_filter_t *B){
	free(B->table);
}

index_t get_bit(bloom_filter_t *B, index_t i)
{
	if (i>(B->size-1)||i<0){
		perror("Error: ");
		return 99;
	}
	index_t key;
	key = 1;
	key = key<<B->size-1-i;
	index_t array;
	array = *B->table & key;
	array = array>>B->size-1-i; 
	return array;
}
void set_bit(bloom_filter_t *B, index_t i)
{
	index_t key;
	key = 1;
	key = key<<B->size-1-i;
	*B->table = *B->table | key;
	/* printf("array=%" PRIu64 "\n", array);
	return array; */
	
}


index_t hash1(bloom_filter_t *B, ourkey_t key)
{
  key = ~key + (key << 15); // key = (key << 15) - key - 1;
  key = key ^ (key >> 12);
  key = key + (key << 2);
  key = key ^ (key >> 4);
  key = key * 2057; // key = (key + (key << 3)) + (key << 11);
  key = key ^ (key >> 16);
  return key % B->size;
}

index_t hash2(bloom_filter_t *B, ourkey_t key)
{
   printf("argument=%d\n", key);
   key = (key+0x7ed55d16) + (key<<12);
   key = (key^0xc761c23c) ^ (key>>19);
   key = (key+0x165667b1) + (key<<5);
   key = (key+0xd3a2646c) ^ (key<<9);
   key = (key+0xfd7046c5) + (key<<3);
   key = (key^0xb55a4f09) ^ (key>>16);
   return key % B->size;
}

uint64_t main(uint64_t argc, char **argv)
{
	uint64_t argument;
   if( argc>1 ) {
    argument = atoi(argv[1]);
  } else {
    printf("argument needed.\n");
    return -1;
  }
	bloom_filter_t Bloom;
	index_t size_in_bits = 3;
	index_t index1 = 1;

	bloom_init(&Bloom, size_in_bits);
	index_t first_bit;
	first_bit = get_bit(&Bloom,index1); 
	printf("first_bit=%" PRIu64 "\n", first_bit);
	set_bit(&Bloom,index1);
	first_bit = get_bit(&Bloom,index1); 
	printf("first_bit=%" PRIu64 "\n", first_bit);
	//printf("count=%d\n", Bloom->count);
	/* uint64_t len=100;
	uint64_t hasha;
	uint64_t hashb;
	hasha=hash1(argument,len);
	hashb=hash2(argument,len);
	printf("hash1=%d\n", hasha);
	printf("hash2=%d\n", hashb); */

	bloom_destroy(&Bloom);

}