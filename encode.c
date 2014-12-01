#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <assert.h>

// global variables
static int acc = 0;
static int count = 0;
static char * key[257];

// method prototypes
struct Node * genList();
int getFreqs(struct Node * head, FILE * f);
struct Node * insertionSort(struct Node * head, int size);
struct Node * makeTree(struct Node * head, int size);
// testing printTree
//void print_tree(struct Node * head);
struct Node * insert_node(struct Node * head, int size);
void printTree(struct Node * head, FILE * f, char * sofar, int len);
void print_bit(char c, FILE * f);
void encode_txt(FILE * input, FILE * output);

struct Node {
	int freq;
	int item;
	struct Node * right;
	struct Node * left;
	struct Node * next;
};


int main(int argc, char *argv[]) {
	FILE * inFile;
	FILE * outFile = stdout;
	int listSize;

	// variables for printTree function
	char sofar[257];
	int len = 0;

	if (argc == 1) {
		perror("Incorrect number of command line arguments!!");
		exit(EXIT_FAILURE);
	} else if (argc == 3) {
		outFile = fopen(argv[2], "w");
	}

	inFile = fopen(argv[1], "r");
	if (inFile == NULL) {
		perror("Input file cannot be opened!");
		exit(EXIT_FAILURE);
	}

	// generate initial linked list
	struct Node * head = genList();

	// get the frequencies
	listSize = getFreqs(head, inFile);

	// sort the list by frequency using insertion sort
	head = insertionSort(head, listSize);

	// construct tree
	head = makeTree(head, listSize);

	// printTree
	printTree(head, outFile, sofar, len);

	// print the bit string representing EOF
	int l = 0;
	char * ch = key[256];
	while (ch[l] != '\0') {
		print_bit(ch[l], outFile);
		l++;
	}


	// rewind input file pointer to the beginning
	rewind(inFile);


	// encode the text and fprintf to outFile
	encode_txt(inFile, outFile);


	// free key

	l = 0;
	for (; l < 257; l++) {
		free(key[l]);
	}


	fclose(inFile);
	fclose(outFile);
	exit(EXIT_SUCCESS);

}

// generates a linked list of 257 nodes (256 plus one for EOF)
// returns a pointer to the head of the linked list
struct Node * genList() {
	struct Node * rtn;
	struct Node * prev;
	int n;

	for (n = -1; n <= 255; n++) {
		struct Node * NewNode = malloc(sizeof(struct Node));
		NewNode->item = n;
		NewNode->freq = 0;
		NewNode->next = NULL;
		NewNode->left = NULL;
		NewNode->right = NULL;

		if (n == -1) {
			rtn = NewNode;
			prev = NewNode;
		} else {
			prev->next = NewNode;
			prev = NewNode;
		}
	}
	return rtn;
}


// obtains the frequency of each character in the file
// removes nodes with 0 frequency
int getFreqs(struct Node * head, FILE * f) {
	int c;
	int counter = 1;

	while (EOF != (c = fgetc(f))) {
		struct Node * p = head;
		for (int i = -1; i < c; i++) {
			p = p->next;
		}
		p->freq += 1;
	}
	head ->freq = 1;
	
	// take out all nodes with frequency 0

	struct Node * prev = head;
	struct Node * q = head->next;

	while (q) {
		if (q->freq == 0) {
			prev->next = q->next;
			free(q);
			q = prev->next;
		} else {
			counter++;
			prev = q;
			q = q->next;
		}
	}

	return counter;

}


struct Node * insertionSort(struct Node * head, int size) {

	if (size == 1) {
		return head;
	}

	struct Node * list[size];
	struct Node * p = head;
	int i = 0;

	// point each array index to corresponding list nodes
	while (p != NULL) {
		list[i] = p;
		p = p->next;
		i++;
	}


	// insertion sort begins

	int k;
	struct Node * l;
	int j;

	for (k = 1; k < size; k++) {
		l = list[k];
		j = k;

		while (j > 0 && list[j-1]->freq >= l->freq) {
			list[j] = list[j-1];
			j = j - 1;
		}
		list[j] = l;
	}

	// set node pointers to correct orders
	int m;

	for (m = 0; m < size - 1; m++) {
		list[m]->next = list[m+1];
	}
	list[m]->next = NULL;
	head = list[0];

	return head;

}


struct Node * makeTree(struct Node * head, int size) {
	struct Node * p = head;
	int s = size;


	while ( p->next != NULL) {
		
		struct Node * NewNode = malloc(sizeof(struct Node));
		NewNode->next = NULL;
		NewNode->left = p;
		NewNode->right = p->next;
		NewNode->freq = p->freq + p->next->freq;
		p->next = NULL;
		p = NewNode;
		NewNode->next = NewNode->right->next;
		NewNode->right->next = NULL;
		s--;

		assert(NewNode->left->next == NULL);
		assert(NewNode->right->next == NULL);


		if (s == 1) {
			break;
		}
		p = insert_node(p, s);
	}

	return p;
}

// print tree function for testing purposes
/*
void print_tree(struct Node * head) {
	if (head->left == NULL) {
		printf("1");
	} else {
		printf("0");
		print_tree(head->left);
		print_tree(head->right);
	}
}
*/

struct Node * insert_node(struct Node * head, int size) {
	if (size == 1) {
		return head;
	}

	struct Node * list[size];
	struct Node * p = head->next;
	int i = 0;

	for (int t = 0; t < size; t++) {
	    list[t] = NULL;
	} 

	// point each array index to corresponding list nodes
	while (p != NULL) {
		list[i] = p;
		p = p->next;
		i++;
	}

	int j = head->freq;
	i = 0;

	while (i < size-1) {
		if (list[i]->freq < j) {
			i++;
			continue;
		} else {
			break;
		}
	}

	if (i == 0) {
		return head;
	} 
	struct Node * rtn = head->next;
	list[i-1]->next = head;
	head->next = list[i];

	return rtn;

}

// traverses the tree in inorder and sends 0 or 1 to print_bit()
// At the same time, constructs the key from character to encoded version
void printTree(struct Node * head, FILE * f, char * sofar, int len) {
	if (head->left == NULL) {
		int mask = 128;
		int ch = head->item;
		//printf("1");
		print_bit('1', f); // it's a leaf node
		
		// prints the character
		for (int i = 0; i < CHAR_BIT; i++) {
		    if((ch & mask) != 0) {
				//printf("1");
				print_bit('1', f);
		    } else {
		    	//printf("0");
				print_bit('0', f);
		    }
		    mask = mask >> 1; // right shift the mask by 1
		}

		
		sofar[len] = '\0';
		char * temp = (char*)malloc(sizeof(char)*257);
		

		//if ch is EOF, change index to 256 not -1
		if (ch == -1) {
			key[256] = temp;
			strcpy(key[256], sofar);
		} else {
			key[ch] = temp;
			strcpy(key[ch], sofar);
		}

		free(head);

		
	} else {
		//printf("0"); 
		print_bit('0', f); // it's an internal node
		sofar[len] = '0';
		printTree(head->left, f, sofar, len+1);

		sofar[len] = '1';
		printTree(head->right, f, sofar, len+1);

		free(head);
	}
}


// accumulates and prints every CHAR_BIT times
void print_bit(char c, FILE * f) {

	if (count == CHAR_BIT) {
		fprintf(f, "%c", acc);
		count = 0;
		acc = 0;
	}

	count++;
	acc = (acc << 1) + (c - '0');
}


// encodes the input text file
void encode_txt(FILE * input, FILE * output) {
	int c;

	while (EOF != (c = fgetc(input))) {
		char temp[257];
		strcpy(temp, key[c]);
		int i = 0;

		while (temp[i] != '\0') {
			print_bit(temp[i], output);
			i++;
		}
	}

	// after EOF is reached, print encoded EOF
	char temp2[257];
	strcpy(temp2, key[256]);
	int j = 0;
	while (temp2[j] != '\0') {
		print_bit(temp2[j], output);
		j++;
	}

	// pad print_bit with zeros to induce print the remaining bits

	int k = 8;

	while (k > 0) {
		print_bit('0', output);
		k--;
	}

}
