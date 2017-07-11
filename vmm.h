#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include "paging.h"
#include "vector.h"

struct MM{
	int numPages;
	int numFrames;
	int blockSize;
	int policy;
	
	pte* pageTable;
	fte* frameTable;

	struct vector fifoVector; //data structures for replacement policies
	struct vector lruVector;
	int clockHand;

	//keep track of paging statistics.
	int reads;
	int writes;
	int pageFaults;
	int replacements; 
};

#define LINEBUFFER_LENGTH 32

#define NO_FREE_FRAMES -1

struct MM* initMM(int pages_in, int frames_in, int policy_in, int block_in);

void initFrame(struct MM* mm);

void parseFrame(struct MM* mm, char* inputFilename);

void printOperations(struct MM* mm);

void mmClose(struct MM* mm);

void allocateFrame(struct MM* mm, int pageId, int frameId);

int translateAddr(struct MM* mm, int pageId);

int findReplacementFrame(struct MM* mm);

int findFreeFrame(struct MM* mm);

void printPageTable(struct MM* mm);

void printFrameTable(struct MM* mm);
