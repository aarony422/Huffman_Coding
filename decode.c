#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

static int count = 8;
static int curgot = 0;
static int zcount = 0;
static int ocount = 0;

struct Node {
	int freq;
	int item;
	struct Node * right;
	struct Node * left;
};

void makeTree(struct Node * root, FILE * f);
int gimmeBit(FILE * f);
void setEOF(struct Node * root, FILE * f);
int decodeStuff(struct Node * root, FILE * f);


int main(int argc, char *argv[]) {
	FILE * inFile;
	FILE * outFile = stdout;
	int k;
	struct Node * head = malloc(sizeof(struct Node));
	
	printf("?????");
	if(argc == 1) {
		perror("Incorrect number of command line arguments!!");
		exit(EXIT_FAILURE);
	} else if (argc == 3) {
		outFile = fopen(argv[2], "w");
	}
	printf("poopyfarts");
	inFile = fopen(argv[1],"r");
	if (!inFile) {
		perror("Input file cannot be opened !!??");
		exit(EXIT_FAILURE);
	}
	printf("omg");
	//struct Node * head = malloc(sizeof(struct Node));
	printf("making da tree");
	makeTree(head,inFile);
	printf("setting da EOF");
	setEOF(head,inFile);
	printf("decoding da message");
	//int k;
	while (-1 != (k = decodeStuff(head, inFile))) {
		//print value? (k)
		fprintf(outFile,"%c",k);
	}
	
	fclose(inFile);
	fclose(outFile);
	exit(EXIT_SUCCESS);
}

void makeTree(struct Node * root, FILE * f) {
	int i;
	int val = 0;
	i = gimmeBit(f);
	if (i == 1) {
		zcount++;
		//get next 8 bits and calculate value
		for (int k = 0; k < 8; k++){
			val = val << 1;
			val = gimmeBit(f);
		}
		//save in node
		root->item = val;
	} else { //i = 0
		ocount++;
		//create left and right subtrees
		struct Node * l = malloc(sizeof(struct Node));
		root->left = l;
		struct Node * r = malloc (sizeof(struct Node));
		root->right = r;
		//recurse on them
		makeTree(root->left,f);
		makeTree(root->right,f);
	}
	//do i need this ?
	/*
	if (zcount + 1 = ocount) {
		//exit
	}*/
}

int gimmeBit(FILE * f){
	int rtn;
	int mask = 255;
	if (count == CHAR_MAX) {
		count = 0;
		curgot = fgetc(f);
	}
	//ret = msb of curgot by shifting
	rtn = curgot >> (CHAR_MAX - (count + 1)); //not sure if this part is right tbh
	//remove msb of curgot
	//shift it left  by count+1
	count++;
	curgot = curgot << count;
	//and with mask
	curgot = curgot && mask;
	//shift back right count
	curgot = curgot >> count;
	return rtn;
}

void setEOF(struct Node * root, FILE * f) {
	int n;
	if (!root->left) {
		//set item to be EOF ?
		root->item = -1;
	} else {
		n = gimmeBit(f);
		if (n == 0) {
		    setEOF(root->left,f);
		} else { //n = 1
			setEOF(root->right,f);
		}
	}
}

int decodeStuff(struct Node * root, FILE * in) {
	int n;
	int k;
	if (!root->left) {
    	    return root->item;
	} else {
	    n = gimmeBit(in);
	    if (n == 0) {
		k = decodeStuff(root->left, in);
	    } else {
		k =decodeStuff(root->right, in);
	    }
	}
	return k;
}

