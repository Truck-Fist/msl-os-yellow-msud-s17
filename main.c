/*
 * Binary Tree - Team Yellow
 * ---------------------------
 *
 * This program reads a file with several lower-cased words on one line
 * and puts them into a binary tree. The tree is then read from left to
 * right and the words are output to a text file with the number of
 * occurences of each word placed next to the word.
 * */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* This is the size of the buffer that is used to read from the file.
 * This means that no word can be larger than this size. */
#define BUFFER_SIZE 100


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


/*
 * add_word
 *
 * This grabs the word from the buffer and traverses the tree in order
 * to find an identical word and increment its count or find
 * where it should be placed as a new word in the tree.
 *
 * No return value.
 * */
void add_word(struct t_node * test_node) {

    printf("\tAdding word...\n");

    /* This is used to numerically represent the new word. */
    int x;

    /* This is used to numerically represent a word on the tree. */
    int y;

    /* This is used to look through each letter in the word to find
     * one that is unique to that word for the sake of sorting them. */
    int i;
    /* Find the first unique character in each word */
    x = 0;
    y = 0;
    i = 0;
    while(x == y && buffer[i] != '\0'
          && test_node->word[i] != '\0') {

        x = buffer[i] - '0';
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
    if(buffer[i] == '\0' && test_node->word[i] != '\0') {
        x = y - 1;
    } else if(buffer[i] != '\0' && test_node->word[i] == '\0') {
        x = y + 1;
    }

    printf("\t\tComparing test node: %s with new word: %s\n",
           test_node->word, buffer);

    if(x > y) { /* We need to go to the right in the tree. */
        if(test_node->greater == NULL) {
            printf("\tAdding node...\n");
            test_node->greater = (struct t_node *)malloc(sizeof(struct t_node));
            int len = 0;
            while(buffer[len] != '\0') {
                len = len + 1;
            }
            char * word = (char *)malloc(len * sizeof(char));
            strcpy(word, buffer);
            test_node->greater->word = word;
            test_node->greater->count = 1;
            test_node->greater->greater = NULL;
            test_node->greater->less = NULL;
        } else {
            add_word(test_node->greater);
        }
    } else if(x < y) { /* We need to go to the left in the tree. */
        if(test_node->less == NULL) {
            printf("\tAdding node...\n");
            test_node->less = (struct t_node *)malloc(sizeof(struct t_node));
            int len = 0;
            while(buffer[len] != '\0') {
                len = len + 1;
            }
            char * word = (char *)malloc(len * sizeof(char));
            strcpy(word, buffer);
            test_node->less->word = word;
            test_node->less->count = 1;
            test_node->less->greater = NULL;
            test_node->less->less = NULL;
        } else {
            add_word(test_node->less);
        }
    } else { /* The two nodes are the same. */
        test_node->count = test_node->count + 1;
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
    fprintf(output_file, "%s: %d\n", node_to_print->word, node_to_print->count);

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
void output_tree(char num[]) {

    printf("Printing tree to output...\n");

    char outputfilename[] = "myoutput??.txt";
    outputfilename[8] = num[5];
    outputfilename[9] = num[6];

    /* Open the output file and start the recursion. */
    output_file = fopen(outputfilename, "w");
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
    free(node_to_destroy->word);
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
int main(int argc, char **argv) {

    /* Read the file, word by word, and give each word a node. */
    fp = fopen(argv[1], "r");
    char parse[13];
    strcpy(parse, argv[1]);
    printf("Opened file to read: %s\n", parse);

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

        add_word(root);
    }
    fclose(fp);
    printf("Tree created!\n");

    output_tree(parse);

    destroy_tree();
    return 0;
}
