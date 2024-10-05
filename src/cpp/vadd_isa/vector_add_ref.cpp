/*
@desc : vec
@param :
*/


#include <stdint.h>
void vector_add_int_ref(int8_t *dst,
                	int8_t *  src1,
                 	int8_t *  src2,
                 	int len)
{
	for(int i=0; i<len; ++i)
	{
		dst[i] = src1[i] + src2[i];
	}
	
	return;
}
