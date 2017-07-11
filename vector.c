#include "vector.h"

//a data vector data structure with an underlying array for use with the fifo queue and (not super optimized) lru entries
//     T        H    
//[3] [4] [] [] [1] [2]
//= [1,2,3,4]

void initVector(struct vector *myVector, int length){ //create a new vector on the heap and init all status variables
	myVector->headIndex = 0;
	myVector->tailIndex = 0;
	myVector->vectorArray = (int*)malloc(sizeof(int)*length);
	myVector->maxSize = length;
	myVector->size = 0;
}

void clearVector(struct vector *myVector){ //make sure we don't have a memory leak
	free(myVector->vectorArray);
}

void insertTail(struct vector *myVector, int toInsert){ //throw an entry to the end of the vector.  For fifo queue, we add to tail and dequeue from head
	if(myVector->size >= myVector->maxSize){
		printf("vector size exceeded when trying to add %d to tail\n", toInsert);
	}else if(myVector->size == 0){
		myVector->headIndex = 0;
		myVector->tailIndex = 0;
		myVector->vectorArray[0] = toInsert;
		myVector->size++;
	}else{
		myVector->size++;
		int newTail = myVector->tailIndex;
		newTail = translateIndex(myVector,newTail+1);
		myVector->vectorArray[newTail] = toInsert;
		myVector->tailIndex = newTail;
	}
}

void insertHead(struct vector *myVector, int toInsert){ //add to head of vector.
	if(myVector->size >= myVector->maxSize){
		printf("vector size exceeded when trying to add %d to head\n", toInsert);
	}else if(myVector->size == 0){
		myVector->headIndex = 0;
		myVector->tailIndex = 0;
		myVector->vectorArray[0] = toInsert;
		myVector->size++;
	}else{
		myVector->size++;
		int newHead = myVector->headIndex;
		newHead = translateIndex(myVector,newHead-1);
		myVector->vectorArray[newHead] = toInsert;
		myVector->headIndex = newHead;
	}
}

int removeHead(struct vector *myVector){
	if(myVector->size == 0){
		//printf("nothing more to remove from vector\n");
		return VECTOR_EMPTY;
	}
	int toReturn = myVector->vectorArray[myVector->headIndex];
	int newHead = myVector->headIndex;
	newHead = translateIndex(myVector,newHead+1);
	myVector->headIndex = newHead;
	myVector->size--;
	return toReturn;
}

void removeItem(struct vector *myVector, int toRemove){ //Remove the specified item from the list.  O(n)
	int index = myVector->headIndex;
	int count = 0;
	bool reachedItem = 0;
	while(count < myVector->size){
		int newIndex = translateIndex(myVector,index+1);
		if(reachedItem || myVector->vectorArray[index] == toRemove){
			reachedItem = 1;
			myVector->vectorArray[index] = myVector->vectorArray[newIndex];
		}
		index = newIndex;
		count++;
	}
	if(reachedItem){
		removeTail(myVector);
	}
}

void printArray(struct vector *myVector){ //prints the entire underlying array.  Included unused entries.
	int i;
	printf("[ ");
	for (i=0; i<myVector->maxSize; i++){
		printf("%d ", myVector->vectorArray[i]);
	}
	printf("]\n");
}

void printVector(struct vector *myVector){
	int index = myVector->headIndex;
	int count = 0;
	printf("{ ");
	while(count < myVector->size){
		printf("%d ", myVector->vectorArray[index]);
		int newIndex = translateIndex(myVector,index+1);
		count++;
		index = newIndex;
	}
	printf("}\n");
}

int removeTail(struct vector *myVector){
	if(myVector->size == 0){
		//printf("nothing more to remove from vector\n");
		return VECTOR_EMPTY;
	}
	int toReturn = myVector->vectorArray[myVector->tailIndex];
	int newTail = myVector->tailIndex;
	newTail = translateIndex(myVector,newTail-1);
	myVector->tailIndex = newTail;
	myVector->size--;
	return toReturn;
}

int translateIndex(struct vector *myVector, int i){ //Basically just i%maxSize, but we need to have a positive modulo for certain edge cases to make sure nothing crazy happens.
	return (myVector->maxSize + (i % myVector->maxSize)) % myVector->maxSize; //Could simply add maxSize to it and mod that, but I figured it was better to not allow any potential overflow.
}
