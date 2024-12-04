#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int (*comparator) (int a, int b);

void die(const char *message) {
	if (errno) perror(message);
	else printf("ERROR: %s\n", message);
	exit(1);
}

int *bubble_sort(int *numbers, int count, comparator cmp) {
	int temp = 0;
	int *target = malloc(count * sizeof(int));
	if (!target) die("Memory error");

	memcpy(target, numbers, count * sizeof(int));
	for (int i = 0; i < count; i++) {
		for (int j = 0; j < count - 1; j++) {
			if (cmp(target[j], target[j + 1]) > 0) {
				temp = target[j + 1];
				target[j + 1] = target[j];
				target[j] = temp;
			}
		}
	}

	return target;
}

int sorted_order(int a, int b) { return a - b; }
int reverse_order(int a, int b) { return b - a; }

void test_sorting(int *numbers, int count, comparator cmp) {
	int *sorted = bubble_sort(numbers, count, cmp);
	if (!sorted) die("Failed to sort");

	for (int i = 0; i < count; i++)
		printf("%d ", sorted[i]);
	printf("\n");
	free(sorted);
}

int main(int argc, char *argv[]) {
	if (argc < 2) die("Usage: ex18 [numbers]");

	int count = argc - 1;
	int *numbers = malloc(count * sizeof(int));
	if (!numbers) die("Memory error");

	for (int i = 0; i < count; i++)
		numbers[i] = atoi(argv[i + 1]);

	test_sorting(numbers, count, sorted_order);
	test_sorting(numbers, count, reverse_order);

	return 0;
}
