#include "include/main.h"
#include "include/major.h"

/*
 * Write getMajor function
 * void getMajor(char * major);
 * Prints a prompt "Enter your major: " to get an major,
 * Reads in the user's major
 */
void getMajor(char * major){
  	printf("Enter your major: ");
	fgets(major, maxLen, stdin);
	fflush(stdin);
}
