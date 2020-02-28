#include <stdlib.h> //required for NULL
#include <stdio.h>
#include "deltas.h" //function prototypes
#include <string.h>

int main(int argc, char *argv[]){
  if(argc < 4){
    printf("usage: %s <format> <filename>\n",argv[0]);
    printf(" <format> is one of\n");
    printf(" text : text ints are in the given filename\n");
    printf(" int  : binary ints are in the given filename\n");
    printf(" 4bit : 4bit binary ints are in the given filename\n");
    return 1;
  }
  char *format = argv[1];
  char *fname = argv[2];
  char *s_max_height = argv[3];

  int max_height = atoi(s_max_height);

  int data_len = -13;
  int *data_vals = NULL;
  
  if( strcmp("text", format)==0 ){ //if the format is text, call read_text_deltas()
    printf("Reading text format\n");
    data_vals = read_text_deltas(fname, &data_len);
  }
  else if( strcmp("int", format)==0 ){ //if the format is int, call read_int_deltas()
    printf("Reading int format\n");
    data_vals = read_int_deltas(fname, &data_len);
  }
  else{ //if it is not text or int, this is an unknown 
    printf("Unknown format '%s'\n",format);
    return 1;
  }

  if(data_vals != NULL){
    print_graph(data_vals, data_len, max_height);
    free(data_vals); //memory allocated by read function is free()'d in main()
  }
  else{
    printf("Returned NULL pointer\n");
    printf("Read %d ints\n",data_len);
  }

  
  return 0;
}




  

