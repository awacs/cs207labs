#include <stdint.h>
#include <stdio.h>
uint64_t hash1(uint64_t key, uint64_t len)
{
  key = ~key + (key << 15); // key = (key << 15) - key - 1;
  key = key ^ (key >> 12);
  key = key + (key << 2);
  key = key ^ (key >> 4);
  key = key * 2057; // key = (key + (key << 3)) + (key << 11);
  key = key ^ (key >> 16);
  return key % len;
}

uint64_t hash2(uint64_t key, uint64_t len)
{
   printf("argument=%d\n", key);
   key = (key+0x7ed55d16) + (key<<12);
   key = (key^0xc761c23c) ^ (key>>19);
   key = (key+0x165667b1) + (key<<5);
   key = (key+0xd3a2646c) ^ (key<<9);
   key = (key+0xfd7046c5) + (key<<3);
   key = (key^0xb55a4f09) ^ (key>>16);
   printf("argument=%d\n", key);
   key = key % len;
   printf("argument=%d\n", key);
   printf("len=%d\n", len);
   return key;
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
	uint64_t len=100;
	uint64_t hasha;
	uint64_t hashb;
	hasha=hash1(argument,len);
	hashb=hash2(argument,len);
	printf("hash1=%d\n", hasha);
	printf("hash2=%d\n", hashb);
	
}