#include <stdio.h>
#include <stdlib.h>

int *makeArray(int arraySize) {
	int val;
	int *Array = (int *) malloc(arraySize * sizeof(int));
	for (int i = 0; i < arraySize; i++) {
		printf("Enter a number for index %d: ", i);
		scanf("%d", &val);
		printf("Array[%d] addy: %p\n", i, &Array[i]);
		Array[i] = val;
		printf("Array[%d] value: %d\n", i, Array[i]);
	}
	return Array;
}

int main(){

	int howmanyInts;
	printf("How many integers do you want to allocate space for? ");
	scanf("%d", &howmanyInts);
	
	int *newArray = makeArray(howmanyInts);
	
	printf("\t\tVirtual Memory Location   Integer value\n");
	
	for (int i = 0; i < howmanyInts; i++) 
		printf("\t\t%p \t\t\t%d\n", &newArray[i], newArray[i]);

	free(newArray);

	return 0;
}
