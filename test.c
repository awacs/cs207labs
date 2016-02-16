#include <stdint.h>
#include <stdio.h>
#include <inttypes.h>
int get_bit(int index, uint64_t array)
{
	uint64_t key;
	key = 1;
	key = key<<63-index;
	array = array & key;
	array = array>>63-index;
	return array;
	
}
uint64_t set_bit(int index, uint64_t array)
{
	uint64_t key;
	key = 1;
	key = key<<63-index;
	printf("key=%" PRIu64 "\n", key);
	array = array | key;
	printf("array=%" PRIu64 "\n", array);
	return array;
	
}
int main(uint64_t argc, char **argv)
{
	int ans;
	ans=get_bit(62,2);
	uint64_t array;
	array=set_bit(61,2);
	printf("ans=%d\n", ans);
	printf("array=%" PRIu64 "\n", array);
}