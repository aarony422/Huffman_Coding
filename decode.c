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
void byebyeTree(struct Node * root);

int main(int argc, char *argv[]) {

	FILE * inFile;
	FILE * outFile = stdout;
	int k;
	struct Node * head = malloc(sizeof(struct Node));
	head->left = NULL;
	head->right = NULL;
	
	
	if(argc == 1) {
		perror("Incorrect number of command line arguments!!");
		exit(EXIT_FAILURE);
	} else if (argc == 3) {
		outFile = fopen(argv[2], "w");
	}
	
	inFile = fopen(argv[1],"r");
	if (!inFile) {
		perror("Input file cannot be opened !!??");
		exit(EXIT_FAILURE);
	}
	
	makeTree(head,inFile);
	
	setEOF(head,inFile);
	//printf("where is cheese ??");
	
	k = decodeStuff(head,inFile);
	//printf("%d\n",k);
	while (-1 != k) {
		//print value? (k)
		//printf("hi");
		fprintf(outFile,"%c",k);
		k = decodeStuff(head,inFile);
	}
	
	byebyeTree(head);
	
	fclose(inFile);
	fclose(outFile);
	exit(EXIT_SUCCESS);
}

void byebyeTree(struct Node * root) {
	if (!root->left) {
		free(root);
	} else {
		byebyeTree(root->left);
		byebyeTree(root->right);
		free(root);
	}
}

void makeTree(struct Node * root, FILE * f) {
	int i;
	int val = 0;
	i = gimmeBit(f);
	if (i) {
		zcount++;
		//get next 8 bits and calculate value
		for (int k = 0; k < 8; k++){
			val = val << 1;
			val = val + gimmeBit(f);
		}
		//save in node
		root->item = val;
	} else { //i = 0
		ocount++;

		//create left and right subtrees
		struct Node * l = malloc(sizeof(struct Node));
		l->left = NULL;
		l->right = NULL;
		root->left = l;
		struct Node * r = malloc (sizeof(struct Node));
		r->left = NULL;
		r->right = NULL;
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
	if (count == CHAR_BIT) {
		count = 0;
		curgot = fgetc(f);
	}
	//ret = msb of curgot by shifting
	rtn = curgot >> 7; 
	rtn &= 1;
	//shift it left  by count+1
	count++;
	curgot = curgot << 1;
	
	if(rtn) {
		//printf("1");
	} else {
		//printf("0");
	}	    
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
			setEOF(root->left, f);
		} else { //n = 1
			setEOF(root->right, f);
		}
	}
}

int decodeStuff(struct Node * root, FILE * in) {
	int n;
	int k;
	if (!root->left) {
	    //printf("%d\n",root->item);
    	    return root->item;
	} else {
	    n = gimmeBit(in);
	    if (n == 0) {
		    k = decodeStuff(root->left, in);
	    } else {
		    k = decodeStuff(root->right, in);
	    }
	}
	return k;
}

