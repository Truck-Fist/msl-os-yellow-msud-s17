/*
 * Binary Tree - Michael Brown
 * ---------------------------
 * 
 * This program reads a file with several lower-cased words on one line
 * and puts them into a binary tree. The tree is then read from left to
 * right and the words are output to a text file with the number of
 * occurences of each word placed next to the word.
 * */


#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

/* This is the size of the buffer that is used to read from the file.
 * This means that no word can be larger than this size. */
#define BUFFER_SIZE 100

/* This is the index of the input file that is to be read. It is also
 * used to create the output file. */
#define INPUT_IDEX "02"


/*
 * This is the structure for each particular node in the tree.
 * It contains an integer that counts how many times a particular word
 * appeared in the file, the word itself, and pointers to the node
 * just greater than it and just less than it.
 * */
struct t_node {
    int count;
    char * word;
    struct t_node * less;
    struct t_node * greater;
};


/*
 * This is the structure for the tree itself.
 * Right now it only has a pointer to the root, but it is still here
 * as this is likely to be expanded on if this project ever got more
 * complex.
 * */
struct tree {

    struct t_node * root;
};

struct tree b_tree;        /* Create the tree itself. */
FILE *fp;                   /* The pointer to the input file. */
FILE *output_file;          /* The pointer to the output file. */
char buffer[BUFFER_SIZE];   /* Where the word is placed when read. */
char * input_name;          /* The eventual name of the input file. */
char * output_name;         /* The eventual name of the output file. */


/*
 * add_node
 * 
 * This grabs the word from the buffer and places it inside of its own
 * node. After that, it traverses the tree in order to find where it
 * should be placed.
 * 
 * No arguments or return value.
 * */
void add_node() {
	
    printf("\tAdding node...\n");
    struct t_node * test_node;
    struct t_node * new_node;
    
    /* This gets set to 1 when the node finds its place. */
    int placement_okay;
    
    /* This is used to numerically represent the new word. */
    int x;
    
    /* This is used to numerically represent a word on the tree. */
    int y;
    
    /* This is used to look through each letter in the word to find
     * one that is unique to that word for the sake of sorting them. */
    int i;

	/* Create the new node. */
    new_node = (struct t_node *)malloc(sizeof(struct t_node));
    int len = 0;
    while(buffer[len] != '\0') {
		
		len = len + 1;
	}
    new_node->word = (char *)malloc(len * sizeof(char));
    strcpy(new_node->word, buffer);
    new_node->count = 1;
    new_node->greater = NULL;
    new_node->less = NULL;

    /* Find out where to place this new node in the tree */
    test_node = b_tree.root;
    placement_okay = 0;
    while(placement_okay == 0) {
	
        /* Find the first unique character in each word */
        x = 0;
        y = 0;
        i = 0;
        while(x == y && new_node->word[i] != '\0'
        && test_node->word[i] != '\0') {
	
            x = new_node->word[i] - '0';
            y = test_node->word[i] - '0';
            i = i + 1;
        }
        
        /* If the new word happens to be a shorter version of one
         * that exists already, force it to be treated as though it were
         * lower in the alphabet.
         * 
         * Example:
         * Alternate and Alter
         * Without the following check, they would be counted as the
         * same word.
         * */
        if(new_node->word[i] == '\0' && test_node->word[i] != '\0') {
			
			x = -1;
		}

        printf("\t\tComparing test node: %s with new word: %s\n",
        test_node->word, new_node->word);

        if(x > y) { /* We need to go to the right in the tree. */
            if(test_node->greater == NULL) {
                placement_okay = 1;
                test_node->greater = new_node;
            } else {
                test_node = test_node->greater;
            }
        } else if(x < y) { /* We need to go to the left in the tree. */
            if(test_node->less == NULL) {
                placement_okay = 1;
                test_node->less = new_node;
            } else {
                test_node = test_node->less;
            }
        } else { /* The two nodes are the same. */
            placement_okay = 1;
            test_node->count = test_node->count + 1;
        }
    }
}


/*
 * output_lsr
 * 
 * This outputs the left node's word, then the word of the node passed
 * to it, and then the right node's word. This is a recursive method.
 * 
 * Arguments:
 *     node_to_print: This is the node that is to be traversed and
 *         printed.
 * 
 * No return value.
 * */
void output_lsr(struct t_node * node_to_print) {
		
    /* If it has a left node, output that, first. */
    if(node_to_print->less != NULL) {
	
        output_lsr(node_to_print->less);
    }

    /* Output the node's own word. */
    printf("\tPlacing %s into file\n", node_to_print->word);
    fprintf(output_file, node_to_print->word);
    fprintf(output_file, ": %d\n", node_to_print->count);

    /* If the node has a right node, output it now. */
    if(node_to_print->greater != NULL) {
	
        output_lsr(node_to_print->greater);
    }
}


/*
 * output_tree
 * 
 * This initiates the printing of the tree to a text file. It opens the
 * file, starts the recursion with the root, and then closes the text
 * file.
 * 
 * No arguments or return value.
 * */
void output_tree() {

    printf("Printing tree to output...\n");
    
    /* Compose the name of the output file. */
    output_name = (char *)malloc(13 * sizeof(char));
    strcpy(output_name, "output");
    strcat(output_name, INPUT_IDEX);
    strcat(output_name, ".txt");
    
    /* Open the output file and start the recursion. */
    output_file = fopen(output_name, "w");
    printf("\tOutput file opened\n");
    output_lsr(b_tree.root);
    fclose(output_file);
}


/*
 * destroy_node
 * 
 * This frees the memory for one particular node and recursively calls
 * itself for the left and right nodes of that particular node.
 * 
 * Arguments:
 *     node_to_destroy: The node to be traversed and then destroyed
 * 
 * No return value.
 * */
void destroy_node(struct t_node * node_to_destroy) {

    printf("\tDestroying %s\n", node_to_destroy->word);
    if(node_to_destroy->less != NULL) {
		
        destroy_node(node_to_destroy->less);
	}
    if(node_to_destroy->greater != NULL) {
	
        destroy_node(node_to_destroy->greater);
    }
    free(node_to_destroy);
}


/*
 * destroy_tree
 * 
 * This initiates the detruction of the entire tree starting with the
 * root.
 * 
 * No arguments or return value.
 * */
void destroy_tree() {

    printf("Destroying tree\n");
    destroy_node(b_tree.root);
}


/*
 * main
 * 
 * The main method opens the text file, reads in every word, creates
 * the tree, prints it to a text file, and then destroys the tree.
 * 
 * No arguments or return value.
 * */
int main() {

    /* Compose the name of the input file. */
    input_name = (char *)malloc(12 * sizeof(char));
    strcpy(input_name, "input");
    strcat(input_name, INPUT_IDEX);
    strcat(input_name, ".txt");
    
    /* Read the file, word by word, and give each word a node. */
    printf("Opening file to read: %s\n", input_name);
    fp = fopen(input_name, "r");

    /* The first word becomes the root. */
    printf("Creating root...\n");
    fscanf(fp, "%s", buffer);
    struct t_node * root;
    root = (struct t_node *)malloc(sizeof(struct t_node));
    root->count = 1;
    int len = 0;
    while(buffer[len] != '\0') {
		
		len = len + 1;
	}
    root->word = (char *)malloc(len * sizeof(char));
    strcpy(root->word, buffer);
    root->greater = NULL;
    root->less = NULL;
    b_tree.root = root;

    /* Read in the remaining words. */
    printf("Reading remaining words...\n");
    while(fscanf(fp, "%s", buffer) != EOF) {
	
        add_node();
    }
    fclose(fp);
    printf("Tree created!\n");

    output_tree();

    destroy_tree();

    return 0;
}
