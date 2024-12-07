#include <stdio.h>
#include <string.h>
#include "debug.h"

#define MAX_DATA 100

typedef enum {
	BLUE, GREEN, BROWN, BLACK, OTHER
} EyeColor;

const char *EYE_COLOR_NAMES[] = {
	"Blue", "Green", "Brown", "Black", "Other"
};

typedef struct {
	int age;
	char first_name[MAX_DATA];
	char last_name[MAX_DATA];
	EyeColor eyes;
	float income;
} Person;

int main(int argc, char *argv[]) {
	Person you = { .age = 0 };
	char *in = NULL;

	printf("What's your first name? ");
	in = fgets(you.first_name, MAX_DATA - 1, stdin);
	check(in != NULL, "Failed to read first name");
	// Set the terminal symbol from \n to \0
	you.first_name[strlen(you.first_name) - 1] = '\0';

	printf("What's your last name? ");
	in = fgets(you.last_name, MAX_DATA - 1, stdin);
	check(in != NULL, "Failed to read last name");
	you.last_name[strlen(you.last_name) - 1] = '\0';

	printf("How old are you? ");
	int rc = fscanf(stdin, "%d", &you.age);
	check(rc > 0, "Age should be a positive number");

	printf("What color are your eyes:\n");
	for (int i = 0; i <= OTHER; i++)
		printf("%d) %s\n", i + 1, EYE_COLOR_NAMES[i]);
	printf("> ");

	int eyes = -1;
	rc = fscanf(stdin, "%d", &eyes);
	check(rc > 0, "Input should be a positive number");

	you.eyes = eyes - 1;
	check(you.eyes <= OTHER && you.eyes >= 0, "Not a valid option");

	printf("How much do you make an hour? ");
	rc = fscanf(stdin, "%f", &you.income);
	check(rc > 0, "Income should be a float");

	printf("----- RESULTS -----\n");
	printf("Name: %s %s\n", you.first_name, you.last_name);
	printf("Age: %d\n", you.age);
	printf("Eyes: %s\n", EYE_COLOR_NAMES[you.eyes]);
	printf("Income: %f\n", you.income);

	return 0;
error:
	return 1;
}
