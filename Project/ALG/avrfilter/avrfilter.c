#include "avrfilter.h"
void avrfilter(float *old_data,float *new_data)
{
	*new_data = *old_data * 0.5 + *new_data * 0.5;
	*old_data = *new_data;
}


