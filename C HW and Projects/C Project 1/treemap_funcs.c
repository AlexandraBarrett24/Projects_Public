#include <stdlib.h> //required for NULL
#include <stdio.h>

#include <string.h>
#include "treemap.h"

// treemap_funcs.c: Provides a small library of functions that operate on
// binary search trees mapping strings keys to string values.


// Initialize the given tree to have a null root and have size 0.
void treemap_init(treemap_t *tree){
	tree->size = 0;	
	tree->root = NULL;
}


// Inserts given key/value into a binary search tree. Uses an
// ITERATIVE (loopy) approach to insertion which starts a pointer at
// the root of the tree and changes its location until the correct
// insertion point is located. If the key given already exists in the
// tree, no new node is created; the existing value is changed to the
// parameter 'val' and 0 is returned.  If no node with the given key
// is found, a new node is created and with the given key/val, added
// to the tree, and 1 is returned. Makes use of strcpy() to ease
// copying characters between memory locations.
int treemap_add(treemap_t *tree, char key[], char val[]){
	node_t *n_ptr; //pointer to iterate through tree
	n_ptr = tree->root;
	node_t *n_past_ptr; //pointer to previous point in the tree
	n_past_ptr = tree->root;
	
	//if the tree is empty malloc a node
	//set its key and val to the passed in key and val and set l and r ptrs to null
	//increment size of tree and return 1
	if(tree->root == NULL){
		n_ptr = malloc(sizeof(node_t));
		strcpy(n_ptr->key, key);	
		strcpy(n_ptr->val, val);
		n_ptr->left = n_ptr->right = NULL;
		tree->root = n_ptr;
		tree->size +=1;
		return 1;	
	}

	int lr = 0; //if lr is 0 it is in the left tree, if 1, it is in the right
	//check to see if the key is in the tree 
	while(n_ptr != NULL){
		int i = strcmp(n_ptr->key, key);
		if(i < 0){ //n_ptr->key is less than key, move into the left subtree
			n_past_ptr = n_ptr;
			n_ptr = n_ptr->left;
			lr = 0;
		}
		else if(i > 0){ //n_ptr->key is more than key, move into the right subtree
			n_past_ptr = n_ptr;
			n_ptr = n_ptr->right;
			lr = 1;
		}
		else{ //key already exists, change val and return 1
			strcpy(n_ptr->val, val);
			return 0;
		}
	}
	
	//if this point is reached, it means that the tree is not empty and the key is not already in the tree
	//need to malloc a node
	//set its key and val to the passed in key and val and set l and r ptrs to null
	//increment size of tree and return 1
	n_ptr =malloc(sizeof(node_t));
	strcpy(n_ptr->key, key);	
	strcpy(n_ptr->val, val);
	n_ptr->left = n_ptr->right = NULL;
	if(lr == 0){
		n_past_ptr->left = n_ptr;
	}
	else{
		n_past_ptr->right = n_ptr;
	}
	tree->size +=1;
	return 1;
}

// Searches the tree for given 'key' and returns its associated
// value. Uses an ITERATIVE (loopy) search approach which starts a
// pointer at the root of the tree and changes it until the search key
// is found or determined not to be in the tree. If a matching key is
// found, returns a pointer to its value. If no matching key is found,
// returns NULL.
char *treemap_get(treemap_t *tree, char key[]){
	if(tree->root == NULL){ //if tree is empty return a null ptr
		return NULL;
	}
	node_t *n_ptr = tree->root; //set a pointer to root of tree
	while(n_ptr != NULL){
		int i = strcmp(n_ptr->key, key);
		if(i < 0){ //n_ptr->key is less than key, move into the left subtree
			n_ptr = n_ptr->left;
		}
		else if(i > 0){ //n_ptr->key is more than key, move into the right subtree
			n_ptr = n_ptr->right;
		}
		else{ //found key, return value
			char *val_ptr = n_ptr->val;
			return val_ptr;
		}
	}
	return NULL; //return null because key is not in tree
}

// Eliminate all nodes in the tree setting its contents empty. Uses
// recursive node_remove_all() function to free memory for all nodes.
void treemap_clear(treemap_t *tree){
	node_remove_all(tree->root); //call node_remove_all() to free the nodes
	//set the root of the tree back to NULL and size back to 0	
	tree->root = NULL;
	tree->size = 0;	
	return;
}

// Recursive helper function which visits all nodes in a tree and
// frees the memory associated with them. This requires a post-order
// traversal: visit left tree, visit right tree, then free the cur
// node.
void node_remove_all(node_t *cur){
	//if the cur ptr is not NULL, go into each subtree and call func again, then free cur ptr
	if (cur != NULL) {
		node_remove_all(cur->left);
		node_remove_all(cur->right);
		free(cur);
	}
}

// Prints the key/val pairs of the tree in reverse order at differing
// levels of indentation which shows all elements and their structure
// in the tree. Visually the tree can be rotated clockwise to see its
// structure. See the related node_print_revorder() for additional
// detals.
void treemap_print_revorder(treemap_t *tree){
	node_print_revorder(tree->root, 0); //call node_print_revorder()
}

// Recursive helper function which prints all key/val pairs in the
// tree rooted at node 'cur' in reverse order. Traverses right
// subtree, prints cur node's key/val, then traverses left tree.
// Parameter 'indent' indicates how far to indent (2 spaces per indent
// level).
void node_print_revorder(node_t *cur, int indent){
	int tab_count = indent; //this is how much a given node should be indented
	if(cur == NULL){ //once cur points to null we have reached the end of the tree, return
		return;
	}
	if(cur->left != NULL){ //recursing to the left, increase tab_count by 1 because it is a subtree
		node_print_revorder(cur->left, tab_count +1);
	}
	if(cur != NULL){ //print the key and val at cur with the correct amount of tabs
		for(int i = 0; i < tab_count; i++){
			printf("  ");
		}
		printf("%s -> %s\n", cur->key, cur->val);
	}
	if(cur->right != NULL){ //recursing to the right, increase tab_count by 1 because it is a subtree
		node_print_revorder(cur->right, tab_count + 1);
	}
}

// Print all the data in the tree in pre-order with indentation
// corresponding to the depth of the tree. Makes use of
// node_write_preorder() for this.
void treemap_print_preorder(treemap_t *tree){
	//call node_write_preorder with stdout as the file so that it prints to the terminal
	node_write_preorder(tree->root, stdout, 0); 
}

// Saves the tree by opening the named file, writing the tree to it in
// pre-order with node_write_preorder(), then closing the file.
void treemap_save(treemap_t *tree, char *fname){	
	FILE *fout = fopen(fname,"w"); //open a file to write in
	if(fout == NULL){ //if the fout ptr is null, the file cannot be opened, print message and return
		printf("file could not be opened\n");
		return;
	}
	node_write_preorder(tree->root, fout, 0); //call node_write_preorder with file
	fclose(fout); //close file
}

// Recursive helper function which writes/prints the tree in pre-order
// to the given open file handle. The parameter depth gives how far to
// indent node data, 2 spaces per unit depth. Depth increases by 1 on
// each recursive call. The function prints the cur node data,
// traverses the left tree, then traverses the right tree.
void node_write_preorder(node_t *cur, FILE *out, int depth){
	int tab_count = depth; //this is how much a given node should be indented
	if(cur == NULL){ //once cur points to null we have reached the end of the tree, return
		return;
	}
	if(cur != NULL){ //print the key and val at cur with the correct amount of tabs
		for(int i = 0; i < tab_count; i++){
			fprintf(out, "     ");
		}
		fprintf(out, "%s %s\n", cur->key, cur->val); //instead of printing it, need to put it in the file
	}
	if(cur->right != NULL){ //recursing to the right, increase tab_count by 1 because it is a subtree
		node_write_preorder(cur->right, out, tab_count +1);
	}
	if(cur->left != NULL){ //recursing to the left, increase tab_count by 1 because it is a subtree
		node_write_preorder(cur->left, out, tab_count +1);
	}		
}

// Clears the given tree then loads new elements to it from the
// named. Repeated calls to treemap_insert() are used to =strings read
// from the file.  If the tree is stored in pre-order in the file, its
// exact structure will be restored.  Returns 1 if the tree is loaded
// successfully and 0 if opening the named file fails in which case no
// changes are made to the tree.
int treemap_load(treemap_t *tree, char *fname ){
	FILE *fin = fopen(fname, "r"); //open the file with fopen()	
	//check to see file can be opened and if not print message and return 0
	if(!fin){
		printf("ERROR: could not open file '%s'\n", fname);
		return 0;
	}
	else{
		treemap_clear(tree); //clear current tree
		while (1){ //until EOF is reached, add elements from the file into the tree by scanning file
			char key[127];
			char val[127];	
			int result = fscanf(fin, "%s %s", &key[0], &val[0]); 
			treemap_add(tree, key, val);
			if(result == EOF){
				break;	// break out of loop at end of file
			}
		}
		fclose(fin); //close file
		return 1; //return 1 because tree loaded successfully
	}
}
