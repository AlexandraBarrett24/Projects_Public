#include <stdlib.h> //required for NULL
#include <stdio.h>
#include "deltas.h" //function prototypes
#include <math.h> //so that i can round
#include <string.h> 

void print_graph(int *data, int len, int max_height){
	//find max and min
	int max = data[0];
	int min = data[0];	
	for(int i =1; i<len; i++){
		if(data[i] < min){
			min = data[i];
		}
		if(data[i] > max){
			max = data[i];
		}	
	}
	//calculate statistics
	int range = max-min;
	double f_range = (double)range;
	double f_max_height = (double)max_height;
	double units_per_height = f_range / f_max_height;
	
	//print initial statistics
	printf("length: %d\n", len);
	printf("min: %d\n", min);
	printf("max: %d\n", max);
	printf("range: %d\n", range);
	printf("max_height: %d\n", max_height);
	printf("units_per_height: %.2f\n", units_per_height);

	//print the top axis
	printf("     ");
	for(int i = 0; i< len; i++){
		if(i%5 == 0){			
			printf("+");
		}
		else{
			printf("-");
		}
	}
	printf("\n");
	
	//print the data and y axis
	for(int level = max_height; level >= 0; level--){
		double f_y_axis = (double)min + (level * units_per_height);		
		int i_y_axis = (int)f_y_axis;
		printf("%3d |", i_y_axis); //print values for y axis
		//print data in graph if the value in the array >= the axis level, otherwise print a space
		for(int i =0; i< len; i++){
			if(data[i] >= i_y_axis){
				printf("X");			
			}
			else{
				printf(" ");
			}
		}
	printf("\n");
	}

	//print the bottom axis
	printf("     ");
	for(int i = 0; i< len; i++){
		if(i%5 == 0){			
			printf("+");
		}
		else{
			printf("-");
		}
	}
	printf("\n");
	printf("     ");

	//print an index reference every 5 units.
	for(int i = 0; i < len; i+=5){ 
		printf("%-5d", i);
	}
	printf("\n");
}
