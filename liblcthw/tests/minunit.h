#undef NDEBUG
#ifndef _minunit_h
#define _minunit_h

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "../src/lcthw/debug.h"

#define mu_suite_start() char *message = NULL

#define mu_assert(test, message) if (!(test)) {\
	log_err(message); return message; }

#define mu_run_test(test) debug("\n-----%s", " " #test); \
	message = test(); tests_run++; if (message) return message;

#define RUN_TESTS(name) int main(int argc, char *argv[]) {\
	argc = 1;\
	debug("----- RUNNING: %s", argv[0]);\
	printf("-----\nRUNNING: %s\n", argv[0]);\
	struct timeval tic, toc;\
	gettimeofday(&tic, NULL);\
	char *result = name();\
	if (result != 0) {\
		printf("FAILED: %s\n", result);\
	}\
	else {\
		printf("ALL TESTS PASSED\n");\
	}\
	gettimeofday(&toc, NULL);\
	double time_taken = toc.tv_sec + toc.tv_usec / 1e6 - tic.tv_sec - tic.tv_usec / 1e6;\
	printf("Tests run: %d\n", tests_run);\
	printf("Time taken: %f seconds\n", time_taken);\
	exit(result != 0);\
}

int tests_run;

#endif
