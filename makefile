#CS475 Homework #8
#Ethan Russell

FILES = main.c vmm.c paging.c vector.c
OUTPUT = vmm

all:
	gcc -Wall $(FILES) -lm -o $(OUTPUT)

clean:
	rm -f *.o *~ $(OUTPUT)*
