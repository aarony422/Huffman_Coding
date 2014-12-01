#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int main() {
	int c = -1;
	int mask = 128;
	
	int i = 0;

	for(; i <CHAR_BIT; i++) {
		if((c & mask) != 0) {
			printf("%c", '1');
		} else {
			printf("%c", '0');
		}
		mask = mask >> 1;
	}
}

