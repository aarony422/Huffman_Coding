#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// global variables
static int acc = 0;
static int count = 0;

// prototyes
struct Node * genList();
int getFreqs(struct Node * poop, FILE * hi);
struct Node * insertionSort(struct Node * head, int size);
struct Node * makeTree(struct Node * head, int size);
void printTree(struct Node * head, FILE * f);
void print_bit(int i, FILE * f);


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

	struct Node * head = genList();
	listSize = getFreqs(head, inFile);
	head = insertionSort(head, listSize);
	head = makeTree(head, listSize);
	printTree(head, outFile);
/*
	struct Node * point;
	point = head;
	while (point) {
		printf("%c%d\n", point->item, point->freq);
		point = point->next;
	}
	printf("%s\n"," ");
	*/

	

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

int getFreqs(struct Node * head, FILE * f) {
	int c;
	int counter = 1;
	while(EOF != (c = fgetc(f))) {
		int i;
		struct Node * p = head;
		for(i = -1; i < c; i++) {
			p = p->next;
		}
		p->freq += 1;
	}
	head->freq = 1;

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

	struct Node ** list = malloc(sizeof(struct Node *)*size);
	struct Node * p = head;
	int i = 0;

	// store node pointers in array to corresponding list nodes
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

	int m;

	for (m = 0; m < size - 1 ; m++) {
		list[m]->next = list[m+1];
	}
	list[m]->next = NULL;

	head = list[0];

	return head;

}

struct Node * makeTree(struct Node * head, int size) {
	struct Node * p = head;
	int s = size;

	while (p->next != NULL) {
		struct Node * newNode = malloc(sizeof(struct Node));
		newNode->left = p;
		newNode->right = p->next;
		newNode->freq = p->freq + p->next->freq;
		p = newNode;
		newNode->next = newNode->right->next;
		s--;
		p = insertionSort(p, s);

	}

	return p; 
}

// traverses the tree in inorder
void printTree(struct Node * head, FILE * f) {
	if (head->left == NULL) {
		print_bit(1, f);
		print_bit(head->item, f);
	} else {
		print_bit(0, f);
		printTree(head->left, f);
		print_bit(0, f);
		printTree(head->right, f);
	}
}


void print_bit(int i, FILE * f) {
	int save = 0;
	if (i <= 1) {
		count++;
		acc = (acc << 1) + i;
	} else {
		save = (255 >> (8 - count)) || i;
		acc = (acc << (8 - count)) + (i >> (8-count));
		
		fprintf(f, "%c", acc);
		acc = save;
	}

	if (count == CHAR_BIT) {
		fprintf(f, "%c", acc);
		count = 0;
		acc = 0;
	}
}

