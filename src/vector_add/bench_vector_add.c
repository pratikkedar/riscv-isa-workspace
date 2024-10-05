// file: bench_vector_add.c
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <riscv_vector.h>
#include <stddef.h>
#include <time.h>

#define SIMD_LEN 128

/** vector addition
 *
 * @param dst address of destination array
 * @param lhs address of left hand side operand array
 * @param rhs address of right hand side operand array
 * @param avl application vector length (array size)
 */


static inline void vector_add(float *dst,
                 const float * restrict lhs,
                 const float * restrict rhs,
                size_t avl)
{
    for (size_t vl; avl > 0; avl -= vl, lhs += vl, rhs += vl, dst += vl)
    {
        // compute loop body vector length from avl
        // (application vector length)
        vl = __riscv_vsetvl_e32m1(avl);
        
        // loading operands
        vfloat32m1_t vec_src_lhs = __riscv_vle32_v_f32m1(lhs, vl);
        vfloat32m1_t vec_src_rhs = __riscv_vle32_v_f32m1(rhs, vl);
        // actual vector addition
        vfloat32m1_t vec_acc = __riscv_vfadd_vv_f32m1(vec_src_lhs,
                                                      vec_src_rhs,
                                                      vl);
        // storing results
        __riscv_vse32_v_f32m1(dst, vec_acc, vl);
    }
}

static inline void vector_add_int(int8_t *dst,
                 		  int8_t * restrict src1,
                 		  int8_t * restrict src2,
                 		  size_t len)
{
    const int incr = (SIMD_LEN / (8 * sizeof(int8_t)));
    size_t vl;
    for(int i=0; i<len; i+=vl)
    {
    	vl = __riscv_vsetvl_e8m2(len - i);
    	
    	vint8m2_t vecArr1 = __riscv_vle8_v_i8m2((src1+i), vl);
        vint8m2_t vecArr2 = __riscv_vle8_v_i8m2((src2+i), vl);
        
        vint8m2_t vec_acc = __riscv_vadd_vv_i8m2(vecArr1, vecArr2, vl);
 
        __riscv_vse8_v_i8m2((dst+i), vec_acc, vl);
    }
}

static inline void vector_add_int_ref(int8_t *dst,
                 		      int8_t * restrict src1,
                 		      int8_t * restrict src2,
                 		      size_t len)
{
	for(int i=0; i<len; ++i)
	{
		dst[i] = src1[i] + src2[i];
	}
	
	return;
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

#if 1
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

#endif

// Defining a default size fot the inputs and output array
// (can be overloaded during compilation with -DARRAY_SIZE=<value>)
#ifndef ARRAY_SIZE
#define ARRAY_SIZE 90
#endif

const float lhs[ARRAY_SIZE] __attribute__ ((aligned (16))) = {1.f};
const float rhs[ARRAY_SIZE] __attribute__ ((aligned (16))) = {2.f};
float dst[ARRAY_SIZE] __attribute__ ((aligned (16))) = {0.f};

uint64_t total_cycles = 0;
uint64_t total_instructions = 0;


/*
  Declairation and Initialisation of source and destination buffers
  Destination buffers initialised to 0.
  
  To Do : Scope of check performance improvement based on buffer alignment
*/
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
    
    // _e8 refers to size of the element in vector, below intrinsic means that given 128 element maximum size of vect possible when 8 bit element is selected range
	size_t vl = __riscv_vsetvl_e8m1(128);
    	printf("vlen : %d \n",vl);
    	printf("simd length is : %d \n", (vl * 8));

	/*
	  Call the ref kernel
	*/    

	vector_add_int_ref(arr4, arr2, arr1, ARRAY_SIZE);

    	double total_time = 0.0;

       	uint64_t start_cycles = read_mcycle();
       	uint64_t start_instructions = read_instret();
       
       	clock_t start = clock();
       	/*
       	  Call vectorized kernel
       	*/
       	vector_add_int(arr3, arr2, arr1, ARRAY_SIZE);
       
       	clock_t end = clock();
       	uint64_t end_cycles = read_mcycle();
       	uint64_t end_instructions = read_instret();
       
       	double exe_time = ((double) (end - start)) / CLOCKS_PER_SEC;
       
       	total_time += exe_time;
      
       	total_cycles += (end_cycles - start_cycles);
       	total_instructions += (end_instructions - start_instructions);

    	printf("avg clock cycles is %f \n", (total_time));
    
    	printf("Cycle Count : %llu\n", (total_cycles));
    	printf("Instruction Count : %llu\n", (total_instructions));

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
