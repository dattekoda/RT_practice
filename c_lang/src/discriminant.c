#include "discriminant.h"
#include "rt_utils.h"

/*
@brief 判別定数を計算する関数
*/
double calc_discriminant(t_abc abc)
{
	return (pow2(abc.y) - abc.x * abc.z);
}
