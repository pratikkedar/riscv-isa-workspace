/*
@desc : vec
@param :
*/

#include <riscv_vector.h>
#include<iostream>
#define SIMD_LEN 128

using namespace std;

void vector_add_int (int8_t *dst,
                     int8_t *  src1,
                     int8_t *  src2,
                     int len)
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
