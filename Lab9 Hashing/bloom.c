#include <stdint.h>
#include <stdio.h>
#include "bloom.h"
#include <inttypes.h>
void bloom_init(bloom_filter_t *B, index_t size_in_bits)
{
    B->table = (uint64_t *) calloc(size_in_bits/sizeof(index_t)+1,sizeof(index_t));
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
	int size = sizeof(index_t)*8;
	int index = i/size;
	int location = i % size;
	index_t key = 1;
	key = key << size-1;
	key = key >> location;
	index_t array;
	array = B->table[index] & key;
	if (array == 0){
		return 0;
	}
	else{
		return 1;
	}
}
void set_bit(bloom_filter_t *B, index_t i)
{
	index_t count;
	int size = sizeof(index_t)*8;
	/* count = bloom_total(B);
	printf("countb=%" PRIu64 "\n", count); */
	int index = i/size;
	int location = i % size;
	index_t key = 1;
	key = key << size-1;
	key = key >> location;
	index_t array;
	index_t value = B->table[index] | key;
	B->table[index] = value;
	
	/* printf("array=%" PRIu64 "\n", array);
	return array; */
	
	/* count = bloom_total(B);
	printf("count=%" PRIu64 "\n", count); */
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
   key = (key+0x7ed55d16) + (key<<12);
   key = (key^0xc761c23c) ^ (key>>19);
   key = (key+0x165667b1) + (key<<5);
   key = (key+0xd3a2646c) ^ (key<<9);
   key = (key+0xfd7046c5) + (key<<3);
   key = (key^0xb55a4f09) ^ (key>>16);
   return key % B->size;
}
void bloom_add(bloom_filter_t *B, ourkey_t k){
	int n=0;
	index_t hasha;
	hasha=hash1(B, k);	
	index_t hashb;
	hashb=hash2(B, k);
	while (n<N_HASHES){
		index_t key;
		key=(hasha+n*hashb) % B->size;
		set_bit(B, key);
		n += 1;
		
	}

	
}
int bloom_check(bloom_filter_t *B, ourkey_t k){
	int n=0;
	int yes = 1;
	index_t hasha;
	hasha=hash1(B, k);	
	index_t hashb;
	hashb=hash2(B, k);
	while (n<N_HASHES){
		index_t key;
		key=(hasha+n*hashb) % B->size;
		yes *= get_bit(B, key);
		n += 1;
	}
	if (yes==1){
		return 1;
	}
	else {
		return 0;
	}
	
}
int bloom_total(bloom_filter_t *B){
	index_t i = 0;
	index_t count = 0;
	
	//printf("size=%" PRIu64 "\n", B->size);
	while (1){
		count += get_bit(B,i);
		/* printf("i=%" PRIu64 "\n", i);
		printf("bit=%" PRIu64 "\n", get_bit(B,i)); */
		i += 1;
		if (i==B->size){
			break;
		}
	}
	return count;
}
	
void main()
{
	
	bloom_filter_t Bloom;
	index_t size_in_bits = 1000;
	bloom_init(&Bloom,size_in_bits);
	/* index_t loc = 23;
	set_bit(&Bloom,loc);
	index_t get = get_bit(&Bloom,loc);
	printf("testget=%" PRIu64 "\n", get);  */
	int max = 70;
	int i = 1;
	index_t count;
	count = bloom_total(&Bloom);
	printf("count=%" PRIu64 "\n", count);
	while (i<=max){
		bloom_add(&Bloom, i);
		i += 1;
		count = bloom_total(&Bloom);
		
	}
	printf("count=%" PRIu64 "\n", count);

	bloom_destroy(&Bloom);
	/* bloom_init(&Bloom,100);
	index_t array[]={0,1,2,3,13,97};
	i=0;
	while (i<6){
		printf("i=%" PRIu64 "\n", array[i]);
		printf("hash1=%" PRIu64 "\n", hash1(&Bloom,array[i]));
		printf("hash2=%" PRIu64 "\n", hash2(&Bloom,array[i]));
		i += 1;
	}
	bloom_destroy(&Bloom); */
	bloom_init(&Bloom,1000);
	index_t numbers1[100];
	index_t numbers2[100];
	i=0;
	while (i<100){
		numbers1[i]=rand()%100000;
		numbers2[i]=rand()%100000;
		i+=1;
	}
	i = 0;
	while (i<100){
		bloom_add(&Bloom, numbers1[i]);
		i+=1;
	}
	count = bloom_total(&Bloom);
	printf("fincount=%" PRIu64 "\n", count);
	i = 0;
	index_t false = 0;
	while (i<100){
		false+=bloom_check(&Bloom,numbers2[i]);
		i+=1;
	}
	printf("falsecount=%" PRIu64 "\n", false);
	bloom_destroy(&Bloom);
	
}