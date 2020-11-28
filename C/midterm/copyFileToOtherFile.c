#include<stdio.h>
#include <stdlib.h> // For exit()
int main() {
	//STEP 1
	char ch;
   	FILE *source, *target;
	source = fopen("sample.txt", "r");
	if( source == NULL )
	{
		printf("Press any key to exit...\n");
		exit(0);
	}
	target = fopen("sample_copy.txt", "w");
	if( target == NULL )
	{
		fclose(source);
		printf("Press any key to exit...\n");
		exit(0);
	}
	while( ( ch = fgetc(source) ) != EOF )
		fputc(ch, target);
	printf("File copied successfully.\n");
	fclose(source);
	fclose(target);
   //STEP 2
	char* text = "sample text 2 4 5";
	int i1, i2, i3;
	char str1[10], str2[10];
	sscanf(text, "%s %s %d %d %d", str1, str2,&i1, &i2, &i3);
	printf("sum: %d\n", i1+i2+i3);
	return 0;
}
