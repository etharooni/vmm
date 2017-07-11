#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define RANDOM 0
#define FIFO 1
#define LRU 2
#define CLOCK 3

#define FREE 1
#define BUSY 0

int pageBits;
int frameBits;

typedef uint32_t pte; //32 bits for a page table entry
typedef uint32_t fte; 

bool getV(pte input);

void setV(pte* input, bool v);

bool getM(pte input);

void setM(pte* input, bool m);

bool getR(pte input);

void setR(pte* input, bool r);

int generateFrameMask();

int getFrame(pte input);

void setFrame(pte* input, int frame);

int generatePageMask();

bool getF(fte input);

void setF(fte* input, bool f);

int getPage(fte input);

void setPage(fte* input, int frame);

void printPageTable();

void printFrameTable();