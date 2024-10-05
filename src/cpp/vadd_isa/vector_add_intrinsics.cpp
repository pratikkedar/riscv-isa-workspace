/*
@desc : vec
@param :
*/

#include <riscv_vector.h>
#include<iostream>
#define SIMD_LEN 128

using namespace std;

/*
vadd_vv (vector add) vint8m2_t vec_acc = __riscv_vadd_vv_i8m2(vecArr1, vecArr2, vl);
vadd_vx (vector-scaler add) vint8m2_t vec_acc = __riscv_vadd_vx_i8m2(vecArr1, 5, vl);

*/

void vadd_variations(int8_t * src, int8_t * dst, int len)
{
	const int incr = (SIMD_LEN / (8 * sizeof(int8_t)));
	size_t vl;
		
	for(int i=0; i<len; i+=vl)
	{
		vl = __riscv_vsetvl_e8m1(len - i);
		
		vint8m1_t vec_input = __riscv_vle8_v_i8m1((src + i), vl);
		
		/*
			Add the vadd intrinsic variation below
		*/
		vint8m1_t vec_output = __riscv_vadd_vx_i8m1(vec_input, 5, vl);
		
		__riscv_vse8_v_i8m1((dst+i), vec_output, vl);
	}
	return;
}

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
