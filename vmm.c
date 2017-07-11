//virtual memory management simulator by Ethan Russell
//Handles four different memory replacement policies: 
//
//random
//	quick and naive - finds random page table entry to replace
//lru
//	finds the least recently used pte and replaces that with the new entry
//fifo
//	keeps a list of pages in memory and returns the first one in the queue to replace
//clock
//	iterates through a list pages until it comes across a pte with a recorded previous page fault when the selected page had a page fault in the past - this gets replaced.  Upon a second page fault, the list will start from the same position in the page table.
#include "vmm.h"

//keep track of two tables - lists of pte and fte
struct MM* initMM(int pages_in, int frames_in, int policy_in, int block_in){
	struct MM* newMM = malloc (sizeof (struct MM));
	newMM->numPages = pages_in;
	newMM->numFrames = frames_in;
	newMM->policy = policy_in;
	newMM->blockSize = block_in;

	newMM->pageTable = 0;
	newMM->frameTable = 0;

	newMM->clockHand = 0;

	newMM->reads = 0;
	newMM->writes = 0;
	newMM->pageFaults = 0;
	newMM->replacements = 0; 
	
	return newMM;
}

void initFrame(struct MM* mm){
	initVector(&(mm->fifoVector), mm->numFrames); //simulator keeps track of all replacement policies, and only executes the one selected
	initVector(&(mm->lruVector), mm->numFrames);
	srand(time(NULL)); //now's as good a time as any to seed random number generator
	//----init----
	pageBits = (int)ceil(log2(mm->numPages*mm->blockSize)); //find the minimum number of bits to store the required number of pages and frames
	frameBits = (int)ceil(log2(mm->numFrames*mm->blockSize));
	if (pageBits > 29){ //three bits of 32 bit page are already used
		printf("not enough precision in PTE to represent all of the required pages\n");
		exit(1);
	}
	if (frameBits > 31){ //one bit of 32 bit frame is for free/busy
		printf("not enough precision in FTE to represent all of the required frames\n");
		exit(1);
	}
	mm->pageTable = malloc(mm->numPages*sizeof(pte)); //throw the page/frame tables on the heap
	mm->frameTable = malloc(mm->numFrames*sizeof(fte));
	int i;
	for(i=0; i<mm->numFrames; i++){ //Could set free bit to !F so I could calloc.  Keeping to instructions.
		mm->frameTable[i] = 0;
		setF(&mm->frameTable[i], FREE);
	}
	for(i=0; i<mm->numPages; i++){
		mm->pageTable[i] = 0;
	}
}

void parseFrame(struct MM* mm, char* inputFilename){
	FILE* fp = NULL;
	fp = fopen(inputFilename, "r");
	if(fp == NULL){
		printf("could not open %s.\n", inputFilename);
		exit(1);
	}
	printf("opened %s\n", inputFilename);
	char line[LINEBUFFER_LENGTH];
	int i=0;
	while (fgets(line, LINEBUFFER_LENGTH, fp) != NULL){ //one line per memory instruction
		if(line[0] == 'R' || line[0] == 'r'){
			mm->reads++;
			printf("BEGIN READ\n");
			char* beginning = &line[1];
			char* end = &line[LINEBUFFER_LENGTH];
			int virtualAddress = strtol(beginning, &end, 16); //use strtol with base 16 to parse hex
			printf("\tVirtual Address: 0x%08X ", virtualAddress);
			int offset = virtualAddress%mm->blockSize; //could request a section of memory in the middle of a page.  Keep track of the offset so we deliver the correct part
			int page = virtualAddress = virtualAddress/mm->blockSize;
			printf("(Page %d + Offset %d)\n", page, offset);
			if(page >= mm->numPages){ //requested a page number outside of our range
				printf("\tSegmentation Fault.  %d >= %d.\n", page, mm->numPages);
			}else{
				int physAddr = (translateAddr(mm,(page)*mm->blockSize)+offset);  //convert to frame with translateAddr(), and add the offset back
				printf("\tPhysical Address: 0x%08X\n", physAddr);
				setR(&mm->pageTable[page], 1);
			}
			printf("END READ\n");	
		}else if (line[0] == 'W' || line[0] == 'w'){//similar process with a write.
			mm->writes++;
			printf("BEGIN WRITE\n");
			char* beginning = &line[1];
			char* end = &line[LINEBUFFER_LENGTH];
			int virtualAddress = strtol(beginning, &end, 16); //use strtol with base 16 to parse hex
			printf("\tVirtual Address: 0x%08X ", virtualAddress);
			int offset = virtualAddress%mm->blockSize;
			int page = virtualAddress = virtualAddress/mm->blockSize;
			printf("(Page %d + Offset %d)\n", page, offset);
			if(page >= mm->numPages){
				printf("\tSegmentation Fault.  %d >= %d.\n", page, mm->numPages);
			}else{
				int physAddr = (translateAddr(mm,((page)*mm->blockSize)))+offset;
				printf("\tPhysical Address: 0x%08X\n", physAddr);
				setR(&mm->pageTable[page], 1);
				setM(&mm->pageTable[page], 1);
			}
			printf("END WRITE\n");				
		}else if (line[0] == 'P' || line[0] == 'p'){
			if(line[1] == 'F' || line[1] == 'f'){
				printFrameTable(mm);
			}else if(line[1] == 'P' || line[1] == 'p'){
				printPageTable(mm);
			}else{
				printf("Could not understand print statement at line %d\n", i);
				exit(1);
			}
		}else{
			printf("Expecting 'R', 'W', 'PP', or 'PF' at beginning of line %d\n", i);
			exit(1);
		}
		i++;
	}
	fclose(fp);
	free(mm->pageTable); //free up the heap
	free(mm->frameTable);
}

void printOperations(struct MM* mm){
	int operations = mm->reads+mm->writes;
	printf("Paging statistics: %d operations (%d%% read, %d%% write), page faults: %d (%d%%), mm->replacements: %d (%d%%)\n", (mm->reads+mm->writes), ((mm->reads*100)/operations), ((mm->writes*100)/operations), mm->pageFaults, ((mm->pageFaults*100)/operations), mm->replacements, ((mm->replacements*100)/operations));
}

void mmClose(struct MM* mm){
	clearVector(&(mm->fifoVector));
	clearVector(&(mm->lruVector));
	free(mm);
}

	
void allocateFrame(struct MM* mm, int pageId, int frameId){ //associates a page with a frame.  Does the required replacement mm->policy record keeping if need be
	setFrame(&mm->pageTable[pageId], frameId);
	setPage(&mm->frameTable[frameId], pageId);
	setF(&mm->frameTable[frameId], BUSY);
	setV(&mm->pageTable[pageId], 1); //validate new page.
	if(mm->policy == FIFO){
		insertTail(&(mm->fifoVector), frameId);
	}
}

int translateAddr(struct MM* mm, int pageId){ //converts a page to a frame in memory, and does the required replacement if need be.
	int frame;
	pte entry = mm->pageTable[pageId];
	if(getV(entry)){ //entry is valid
		frame = getFrame(entry);
		printf("\tTranslation: Frame %d\n", frame);
	}else{
		printf("\tPage Fault: Page %d\n", pageId);
		mm->pageFaults++;
		int nextFreeFrame = findFreeFrame(mm);
		if(nextFreeFrame == NO_FREE_FRAMES){ //no more free frames - must replace existing frame and swap out to disk
			nextFreeFrame = findReplacementFrame(mm);
			int oldPage = getPage(mm->frameTable[nextFreeFrame]);
			bool writeout = getM(mm->pageTable[oldPage]); //if the section of memory hasn't been written to, we don't need to save it to disk
			printf("\tPage replacement: evicted page %d.  Writeout: %d\n", oldPage, writeout);
			setV(&mm->pageTable[oldPage], 0); //invalidate old page.  (swapping from disk)
			allocateFrame(mm, pageId, nextFreeFrame);
			frame = nextFreeFrame;
		}else{ //find a free frame and use it.
			printf("\tNext free frame: %d\n", nextFreeFrame);
			allocateFrame(mm, pageId, nextFreeFrame);
			frame = nextFreeFrame;
		}
	}
	if(mm->policy == LRU){
		removeItem(&(mm->lruVector), frame); //a vector may not be the best data structure, but was chosen so it could be built on top of the existing FIFO code
		insertHead(&(mm->lruVector), frame);
	}else if (mm->policy == CLOCK){
		mm->clockHand = pageId;
	}
	return frame;
}

int findReplacementFrame(struct MM* mm){ //if all of the frames are used, use the selected replacement mm->policy to kick one out (ideally one we don't need to use all that much)
	mm->replacements++;
	int newFrame;
	if(mm->policy == RANDOM){
		newFrame = rand()%mm->numFrames; //choose any old frame.
	}else if(mm->policy == FIFO){
		newFrame = removeHead(&(mm->fifoVector)); //Take the frame we accesed first.
	}else if(mm->policy == LRU){
		newFrame = removeHead(&(mm->lruVector)); //Use the frame not used recently.
	}else if(mm->policy == CLOCK){
		while(getR(mm->pageTable[mm->clockHand])){ //look for a frame that has not been accesed.  If that's not possible, clear the R bit for the pages we've looked through and use one of those if need be
			setR(&mm->pageTable[mm->clockHand], 0);
			mm->clockHand = (mm->clockHand+1)%mm->numPages;
		}
		newFrame = getFrame(mm->pageTable[mm->clockHand]);
		setPage(&mm->frameTable[newFrame], mm->clockHand); //make sure the frame points to the correct page so we don't invalidate the wrong one
	}else{
		printf("invalid replacement mm->policy\n");
		return -1;
	}
	return newFrame;
}

int findFreeFrame(struct MM* mm){ //linear search to find the next free frame.  Could keep track of the free frames on a queue to optimize.
	int i;
	for(i=0; i<mm->numFrames; i++){
		if(getF(mm->frameTable[i])){
			return i;
		}
	}
	return NO_FREE_FRAMES;
}


void printPageTable(struct MM* mm){
	printf("Page\t| V\tM\tR\tFrame\n");
	int i;
	for(i=0; i<mm->numPages; i++){
		bool valid = getV(mm->pageTable[i]);
		bool modify = getM(mm->pageTable[i]);
		bool reference = getR(mm->pageTable[i]);
		int frame = getFrame(mm->pageTable[i]);
		if(valid){
			printf("%d\t| %d\t%d\t%d\t%d\n", i, valid, modify, reference, frame);
		}else{
			printf("%d\t| %d\t%d\t%d\t-\n", i, valid, modify, reference);
		}
	}
}

void printFrameTable(struct MM* mm){
	printf("Frame\t| F\tPage\n");
	int i;
	for(i=0; i<mm->numFrames; i++){
		bool free = getV(mm->frameTable[i]);
		int page = getPage(mm->frameTable[i]);
		if(free){
			printf("%d\t| %d\t-\n", i, free);
		}else{
			printf("%d\t| %d\t%d\n", i, free, page);
		}
	}
}