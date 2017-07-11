#include "paging.h" 

//This file is used for all of the bit manipulation math needed for reading and modifying page and frame table entries.

int frameMask = 0;
int pageMask = 0;

int pageBits = 29; //default to max precision
int frameBits = 31;

bool getV(pte input){
	return 1 & (input >> 31);
}

void setV(pte* input, bool v){
	if(v){
		*input |= (1 << 31);
	}else{
		*input &= ~(1 << 31);
	}
}

bool getM(pte input){
	return 1 & (input >> 30);
}

void setM(pte* input, bool m){
	if(m){
		*input |= (1 << 30);
	}else{
		*input &= ~(1 << 30);
	}
}

bool getR(pte input){
	return 1 & (input >> 29);
}

void setR(pte* input, bool r){
	if(r){
		*input |= (1 << 29);
	}else{
		*input &= ~(1 << 29);
	}
}

int generateFrameMask(){ //3 bits makes a mask 0b0111.  Used to make sure we don't overwrite status bits with our frame number
	if(frameMask == 0){ //make sure to save it so we don't have to calculate every time
		int mask = 0;
		int i;
		for(i=0; i<frameBits; i++){
			mask |= (1 << i);
		}
		frameMask = mask;
	}
	return frameMask;
}

int getFrame(pte input){
	int mask = generateFrameMask();
	return mask & input;
}

void setFrame(pte* input, int frame){
	int mask = generateFrameMask();
	*input = mask & frame;
}

int generatePageMask(){
	if(pageMask == 0){ //make sure to save it so we don't have to calculate every time
		int mask = 0;
		int i;
		for(i=0; i<pageBits; i++){
			mask |= (1 << i);
		}
		pageMask = mask;
	}
	return pageMask;
}

bool getF(fte input){
	return 1 & (input >> 31);
}

void setF(fte* input, bool f){
	if(f){
		*input |= (1 << 31);
	}else{
		*input &= ~(1 << 31);
	}
}

int getPage(fte input){
	int mask = generatePageMask();
	return mask & input;
}

void setPage(fte* input, int frame){
	int mask = generatePageMask();
	*input = mask & frame;
}