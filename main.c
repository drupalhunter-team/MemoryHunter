#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mhDefs.h"
#include "stuDefs.h"

int main()
{
	//Program Desc
	printf("Memory Hunter: A less-than-mediocre ripoff of Valgrind!\n");
	
	//Testing w/ 5 Data Types
	HeapType *heap;
	void  *ptr1, *ptr2, *ptr3, *ptr4, *ptr5;

  	heap = (HeapType *) malloc(sizeof(HeapType));
  	mh_init(heap);

  	ptr1 = mh_alloc(heap, 5*sizeof(PlayerType), "Players");
  	ptr2 = mh_alloc(heap, 10*sizeof(EmployeeType), "Employees");
  	ptr3 = mh_alloc(heap, 8*sizeof(MilitaryType), "Soldiers");
  	ptr4 = mh_alloc(heap, 12*sizeof(InstructorType), "Instructor");
	ptr5 = mh_alloc(heap, 6*sizeof(CellType), "Cell Phones");

  	printf("\nDUMP 1, byte count = %d\n", mh_count(heap));
  	mh_dump(heap);

  	mh_dealloc(heap, ptr1);
  	mh_dealloc(heap, ptr2);
  	mh_dealloc(heap, ptr3);

  	printf("\nDUMP 2, byte count = %d\n", mh_count(heap));
  	mh_dump(heap);

  	mh_collect(heap);

  	printf("\nDUMP 3, byte count = %d\n", mh_count(heap));
  	mh_dump(heap);
  	printf("\n\n");

  	mh_cleanup(heap);
  	free(heap);

  	return 0;
}

//Initializes HeapType Variable and allocates BlockTypeSize*Max_Blocks space
void mh_init(HeapType *heap) {
	heap->numBlocks = 0;
	heap->blocks = (BlockType *) malloc(sizeof(BlockType)*MAX_BLK);	
}

//Clean up function for block array and heap array
//Cleans up any memory still allocated in blocks in case user forgets to use mh_collect before this
void mh_cleanup(HeapType *heap) {
	for (int i=0; i<MAX_BLK; i++) {
		if (heap->blocks[i].rsv == C_TRUE) {
			free(heap->blocks[i].addr);
		}	
	}
	free(heap->blocks);
}

//Allocates n bytes to the first non-reserved block element.
//Sets name and tracks size
//Does not deal with the case where all 16 blocks are reserved
void *mh_alloc(HeapType *heap, int n, char *label) {
	int i = 0;
	for (; i < MAX_BLK; i++) {
		if (heap->blocks[i].rsv != C_TRUE) {	
			heap->blocks[i].addr = malloc(n);
			heap->blocks[i].size = n;
			strcpy(heap->blocks[i].tag, label);
			heap->blocks[i].rsv = C_TRUE;
			heap->numBlocks++;
			break;
		}
	}

	return heap->blocks[i].addr;
}

//Frees the memory located at addr and unreserves that block
void mh_dealloc(HeapType *heap, void *addr) {
	for (int i=0; i<MAX_BLK; i++) {
		if ( heap->blocks[i].rsv != C_TRUE ) {
			continue;
		}else {
			if ( addr == heap->blocks[i].addr ) {
				free(heap->blocks[i].addr);
				heap->blocks[i].rsv = C_FALSE;
			}
		}
	}
}

//Counts and returns the total number of bytes allocated in each block
int mh_count(HeapType *heap) {
	int total_size = 0;
	
	for (int i = 0; i < MAX_BLK; i++) {
		if (heap->blocks[i].rsv != C_TRUE) {
			continue;
		}else {
			total_size += heap->blocks[i].size;
		}
	}
	return total_size;
}

//Output displaying visual info of the name of each reserved block, its size, and its address.
void mh_dump(HeapType *heap) {

	for (int i=0; i<MAX_BLK; i++) {
		if (heap->blocks[i].rsv != C_TRUE) {
			continue;
		} else {
			printf("\n  %10s:   %8d bytes stored at %x", heap->blocks[i].tag, heap->blocks[i].size, heap->blocks[i].addr);
		}
	}
	printf("\n");
}

//Frees all allocated memory within the Heap's Blocks, sets them to unreserved
//Similar to cleanup, however does not deallocate the blocks memory, ie, this is used when you
//simply want a fresh set of blocks
void mh_collect(HeapType *heap) {
	for (int i=0; i<MAX_BLK; i++) {
		if (heap->blocks[i].rsv == C_TRUE) {
			free(heap->blocks[i].addr);
			heap->blocks[i].rsv = C_FALSE;
		}
	}
}
