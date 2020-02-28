#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"
//#include "treemap_funcs.c"

int main(int argc, char *argv[]){
  int echo = 0;                                // controls echoing, 0: echo off, 1: echo on
  if(argc > 1 && strcmp("-echo",argv[1])==0) { // turn echoing on via -echo command line option
    echo=1;
  }

  printf("TreeMap Editor\n");
  printf("Commands:\n");
  printf("  quit:            exit the program\n");
  printf("  print:           shows contents of the tree in reverse sorted order\n");
  printf("  add <key> <val>: inserts the given key/val into the tree, duplicate keys are ignored\n");
  printf("  get <key>:       prints FOUND if the name is in the tree, NOT FOUND otherwise\n");
  printf("  clear:           eliminates all key/vals from the tree\n");
  printf("  preorder:        prints contents of the tree in pre-order which is how it will be saved\n");
  printf("  save <file>:     writes the contents of the tree in pre-order to the given file\n");
  printf("  load <file>:     clears the current tree and loads the one in the given file\n");  
	char cmd[128];
	int success;
	treemap_t *tree = malloc(sizeof(treemap_t));
	treemap_init(tree);

 while(1){
    printf("TM> ");                 // print prompt
    success = fscanf(stdin,"%s",cmd); // read a command
    if(success==EOF){                 // check for end of input
      printf("\n");                   // found end of input
      break;                          // break from loop
    }

    if( strcmp("quit", cmd)==0 ){     // check for quit command
      if(echo){
        printf("quit\n");
      }  
      break;                          // break from loop
    }

    else if( strcmp("print", cmd)==0 ){ // check for print command
      if(echo){
        printf("print\n");
      }	     
	treemap_print_revorder(tree); //call print tree in revorder
    }

    else if( strcmp("add", cmd)==0 ){     // check for add command
	char key[127];
	char val[127];
	fscanf(stdin,"%s %s", key, val);   //read key and val from input   
      if(echo){
        printf("add %s %s\n",key, val);
      }     	
	int t = treemap_add(tree, key, val); //call treemap_add() to add key and val
	if(t == 0){ //if t=0, key already exists and was modified
		printf("modified existing key\n");
	}
    }

    else if( strcmp("get", cmd)==0 ){   // check for get command
	char key[127];
	fscanf(stdin,"%s", key); //read key from input
      if(echo){
        printf("get %s\n", key);
      }
	char *found = treemap_get(tree, key); //call treemap_get() to get val at key
	if(found != NULL){ //if found != NULL, we found the val, output val
		printf("FOUND: %s\n", found);
	}
	else{ //if found == NULL, key is not in the tree, output NOT FOUND
		printf("NOT FOUND\n");
	}
    }

    else if( strcmp("clear", cmd)==0 ){   // check for clear command
      if(echo){
        printf("clear\n");
      }
	treemap_clear(tree); //call treemap_clear() to clear tree
    }

    else if( strcmp("preorder", cmd)==0 ){ // check for preorder command
      if(echo){
        printf("preorder\n");
      }       
    	treemap_print_preorder(tree); //call treemap_print_preorder() to print tree in preorder

    }

    else if( strcmp("save", cmd)==0 ){ // check for save command
	char file[128];
      fscanf(stdin,"%s",file);  //get name of file from input
	treemap_save(tree, file); //call treemap_save() to save tree to file
      if(echo){
        printf("save %s\n", file);
      }        
    }

    else if( strcmp("load", cmd)==0 ){ // check for load command
	char file[128];      
	fscanf(stdin,"%s", file); //get name of file from input
      if(echo){
        printf("load %s\n", file);
      }
	int t = treemap_load(tree, file); //call treemap_load() to load tree at file
	if(t == 0){ //if t=0, the load has failed, output load failed
		printf("load failed\n");
	}

    }

    else{                                 // unknown command 
      if(echo){
        printf("%s\n",cmd);
      }        
	printf("unknown command %s\n",cmd);
    }
  }  

  if (tree != NULL) { //as long as tree is not null, clear the tree and free the mem for tree
    treemap_clear(tree);
    free(tree);
  }

}
