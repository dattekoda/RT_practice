#include "onb.h"

// ジンバルロックのバグを孕む
/*
@param _w 正規化不要
@param _upper 正規化必要
*/
t_onb	construct_w_onb(t_vec3 _w, t_vec3 upper)
{
	t_onb	onb;

	onb.w = normalize(_w);
	onb.u = cross(upper, onb.w);
	onb.v = cross(onb.w, onb.u);
	return (onb);
}
