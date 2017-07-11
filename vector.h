#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define VECTOR_EMPTY -1

struct vector{
	int* vectorArray; //uses an array as fundamental type
	int headIndex;
	int tailIndex;
	int size;
	int maxSize;
};

void initVector(struct vector *myVector, int length);
void clearVector(struct vector *myVector);
void insertTail(struct vector *myVector, int toInsert);
void insertHead(struct vector *myVector, int toInsert);
void removeItem(struct vector *myVector, int toRemove);
int removeHead(struct vector *myVector);
int removeTail(struct vector *myVector);
void printVector(struct vector *myVector);
void printArray(struct vector *myVector);
int translateIndex(struct vector *myVector, int i);