// file: bench_vector_add.c
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <time.h>

#include<bits/stdc++.h>

/*
  Declairation and Initialisation of source and destination buffers
  Destination buffers initialised to 0.
  
  To Do : Scope of check performance improvement based on buffer alignment
*/


uint64_t read_mcycle()
{
  uint64_t cycle_count;
  
  //asm volatile ("csrr %0, mcycle" : "=r" (cycle_count));
  asm volatile ("rdcycle %0" : "=r" (cycle_count));
  
  return cycle_count;
}

uint64_t read_minstret()
{
  uint64_t cycle_count;
  
  //asm volatile ("csrr %0, minstret" : "=r" (cycle_count));
  asm volatile ("rdinstret %0" : "=r" (cycle_count));
  
  return cycle_count;
}

/* return the value of the instret counter
 *
 *  The instret counter counts the number of retired (executed) instructions.
*/
unsigned long read_instret(void)
{
  unsigned long instret;
  asm volatile ("rdinstret %0" : "=r" (instret));
  return instret;
}

#ifndef ARRAY_SIZE
#define ARRAY_SIZE 9000
#endif

int8_t arr1[ARRAY_SIZE] __attribute__ ((aligned (16))) = {1};
int8_t arr2[ARRAY_SIZE] __attribute__ ((aligned (16))) = {2};

int8_t arr3[ARRAY_SIZE] __attribute__ ((aligned (16))) = {0};
int8_t arr4[ARRAY_SIZE] = {0};

int comp_buffs(int8_t *dst1,
	       int8_t *dst2,
               size_t len)
{
	for(int i=0; i<len; ++i)
	{
		if(dst1[i] != dst2[i])
		{
			printf("dst1[i] : %d dst2[i] : %d\n", dst1[i], dst2[i]);
			return 0;
		}
			
	}
	
	return 1;
}

void vector_add_int(int8_t *dst,
                     int8_t *  src1,
                     int8_t *  src2,
                     int len);
                       	   
void vector_add_int_ref(int8_t *dst,
                	int8_t *  src1,
                 	int8_t *  src2,
                 	int len);
int main(void) {
    
    	
    	/*
    	  Initialise the data buffers, src buffers with temp data and dst buffers with 
    	*/
	for(int i=0; i<ARRAY_SIZE; ++i)
	{
		arr1[i] = 100 + i;
		arr2[i] = 200 + i;
		arr3[i] = arr4[i] = 0;  
	}
    
	/*
	  Call the ref kernel
	*/    

	vector_add_int_ref(arr4, arr2, arr1, ARRAY_SIZE);

       	uint64_t start_cycles = read_mcycle();
       	uint64_t start_instructions = read_instret();
       
       	clock_t start = clock();
       	/*
       	  Call vectorized kernel
       	*/
       	vector_add_int_ref(arr3, arr2, arr1, ARRAY_SIZE);
       
       	clock_t end = clock();
       	uint64_t end_cycles = read_mcycle();
       	uint64_t end_instructions = read_instret();

    	printf("timer clock cycles is %f \n", (((double) (end - start)) / CLOCKS_PER_SEC));
    
    	printf("Cycle Count : %llu\n", ((end_cycles - start_cycles)));
    	printf("Instruction Count : %llu\n", (end_instructions - start_instructions));

	/*
	  Test case to check the ref and opt buffers
	*/
	if(!comp_buffs(arr3, arr4, ARRAY_SIZE))
	{
		printf("Test failed");
	}
	else
	{
		printf("Test Passed\n");
	}
    	return 0;
}


/*
benchmark results (Instructions execution)
Data type : 8 bit
Buffer size : 90 elements 

m1 : 5732
m2 : 5699
m4 : 5688
m8 : 5677

Buffer size : 9000 elements

ref : 86668
m1 : 11861
m2 : 8770
m4 : 7219
m8 : 6449
*/
