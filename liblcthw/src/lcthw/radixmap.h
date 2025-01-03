#ifndef _radixmap_h_
#define _radixmap_h_

#include <stddef.h>
#include <stdint.h>

typedef union {
	uint64_t raw;
	struct {
		uint32_t key;
		uint32_t value;
	} data;
} RMElement;

typedef struct {
	size_t max;
	size_t end;
	uint32_t counter;
	RMElement *contents;
	RMElement *temp;
} RadixMap;

RadixMap *RadixMap_create(size_t max);
void RadixMap_destroy(RadixMap *map);
void RadixMap_sort(RadixMap *map);
RMElement *RadixMap_find(RadixMap *map, uint32_t key);
int RadixMap_add(RadixMap *map, uint32_t key, uint32_t value);
int RadixMap_delete(RadixMap *map, RMElement *element);

#endif
