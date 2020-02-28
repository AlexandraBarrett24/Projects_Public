#include <stdlib.h> //required for NULL
#include <stdio.h>
#include "deltas.h" //function prototypes
#include <sys/stat.h> //declares stat()
#include <string.h>

// Reads integers in text delta format from the file named by fname
// and returns an array of them. The first integer in the file gives
// the starting point and subsequent integers are changes from the previous total.	
int *read_text_deltas(char *fname, int *len){
	//open the file with fopen()	
	FILE *fin = fopen(fname, "r");

	//check to see file can be opened
	if(!fin){
		
		*len = -1;
		return NULL;
	}
	else{
	int counter = 0; //count how many ints
	int extra[1]; //pointer to put the value in while we scan
	while (1){
		//use fscanf() to count number of ints
		int result = fscanf(fin, "%d", &extra[0]); 
		if(result == EOF){
			break;	// break out of loop at end of file
		}	
		counter++; 
	}

		//printf("tada %d",*len);
	//allocates an array of appropriate size with malloc
	int *buf = malloc(counter * sizeof(int));
	
	//go back to beginning of file
	rewind(fin);
	int result = fscanf(fin, "%d", &buf[0]); 
	if(result == EOF){
		fclose(fin);
		free(buf);
		*len = -1;
		return NULL;

	}
	rewind(fin);
	//read all ints
	int addition = 0;
	for(int i =0; i<counter; i++){
		fscanf(fin, "%d", &buf[i]);
		addition += buf[i]; //delta computations
		buf[i] = addition;
	}
	//close file
	fclose(fin);



	//need to free the data	

	*len = counter; //point len at counter
	return buf; //return pointer to allocated array
}
}
// Reads integers in binary delta format from the file named by fname
// and returns an array of them.  The first integer in the file gives
// the starting point and subsequent integers are changes from the
// previous total.
int *read_int_deltas(char *fname, int *len){
	
	//use stat() system to determine the file size in bytes
	struct stat sb;  //struct to hold
	int result = stat(fname, &sb);  //unix system call to determine the size of named fiel
	if(result == -1 || sb.st_size < sizeof(int)){
		*len = -1;		
		return NULL;
	}
	int total_bytes = sb.st_size; //size of the file in bytes

	total_bytes = total_bytes/sizeof(int); //determine the number of ints	

	//allocate an array of right size	
	int *values = malloc(total_bytes * sizeof(int));
	//+

//printf("%d",total_bytes);
	//open file
	FILE *fin = fopen(fname, "r");
	//check if file exists
	if(fin == NULL){
		fclose(fin);
		*len = -1;
		return NULL;
	}

	//check if there are no ints in the file (if yes set len to -1 and return NULL
	int res = fread(&values[0], sizeof(int), 1, fin); 
	
	if(res == 0){
		*len = -1;
		fclose(fin);
		return NULL;

	}
	rewind(fin);

	int addition = 0;
	//repeated calls to fread() to read ints into array	
	for(int i = 0; i < total_bytes; i++){
		fread(&values[i], sizeof(int), 1, fin);
		addition += values[i]; //delta computations
		values[i] = addition;
	}
	//close file after reading it
	fclose(fin);

	*len = total_bytes;	//set len to the length of the array
	return values; 	//return pointer to array

}

