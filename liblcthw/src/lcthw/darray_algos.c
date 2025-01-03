#include <bsd/stdlib.h>
#include "darray_algos.h"

int DArray_qsort(DArray *array, DArray_compare cmp) {
	qsort(array->contents, DArray_count(array), sizeof(void *), cmp);
	return 0;
}

int DArray_heapsort(DArray *array, DArray_compare cmp) {
	heapsort(array->contents, DArray_count(array), sizeof(void *), cmp);
	return 0;
}

int DArray_mergesort(DArray *array, DArray_compare cmp) {
	mergesort(array->contents, DArray_count(array), sizeof(void *), cmp);
	return 0;
}
