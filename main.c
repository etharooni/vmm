//CS475 Homework 8 by Ethan Russell
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include "paging.h"

#define NO_FREE_FRAMES -1
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdlib.h>
#include "vmm.h"

#define DEFAULT_PAGES 8
#define DEFAULT_FRAMES 4
#define DEFAULT_BLOCK 1024
#define DEFAULT_REP RANDOM

int main(int argc, char** argv){
	int pages = DEFAULT_PAGES; //set defaults
	int frames = DEFAULT_FRAMES;
	int repPolicy = DEFAULT_REP;
	int blockSize = DEFAULT_BLOCK;
	
	int i;
	int lastArg = 0;
	for(i=1; i<(argc-1); i++){ //don't loop through strings after flag 
		if(strncmp(argv[i], "-p", 2) == 0){ //numer of pages
			pages = atoi(argv[i+1]);
			lastArg = i+1;
			if (pages%2 != 0){
				printf("number of pages must be a multiple of 2.\n");
				exit(1);
			}
		}else if(strncmp(argv[i], "-f", 2) == 0){ //number of frames
			frames = atoi(argv[i+1]);
			lastArg = i+1;
			if (frames%2 != 0){
				printf("number of frames must be a multiple of 2.\n");
				exit(1);
			}
		}else if(strncmp(argv[i], "-h", 2) == 0){ //block size 
			blockSize = atoi(argv[i+1]);
			lastArg = i+1;
			if (blockSize%2 != 0){
				printf("block size must be a multiple of 2.\n");
				exit(1);
			}
		}else if(strncmp(argv[i], "-r", 2) == 0){ //rep policy
			lastArg = i+1;
			if(strcmp(argv[i+1], "random") == 0){
				printf("Page Replacement Policy: Random\n");
				repPolicy = RANDOM;
			}else if(strcmp(argv[i+1], "fifo") == 0){
				printf("Page Replacement Policy: FIFO\n");
				repPolicy = FIFO;
			}else if(strcmp(argv[i+1], "lru") == 0){
				printf("Page Replacement Policy: LRU\n");
				repPolicy = LRU;
			}else if(strcmp(argv[i+1], "clock") == 0){
				printf("Page Replacement Policy: Clock\n");
				repPolicy = CLOCK;
			}else{
				printf("Didn't understand rep policy.  Expecting \"random\", \"fifo\", \"lru\", or \"clock\".\n");
				exit(1);
			}
		}
	}
	if(argc-lastArg < 2){
		printf("need input file\n");
		exit(1);
	}
	struct MM* mm = initMM(pages, frames, repPolicy, blockSize);
	initFrame(mm);
	parseFrame(mm,argv[argc-1]);
	
	printOperations(mm);
	
	mmClose(mm);
	
	return 0;
}
