#include "onb.h"

/*
@brief 正規直交基底をつくる
@param _w 正規化不要
*/
t_onb	construct_w_onb(t_vec3 _w)
{
	t_onb	onb;
	t_vec3	upper;

	upper = construct_vec(0, 1, 0);
	if (length_squared_vec(cross(upper, onb.w)) < 0.001)
		upper = construct_vec(0, 0, 1);
	onb.w = normalize(_w);
	onb.u = cross(upper, onb.w);
	onb.v = cross(onb.w, onb.u);
	return (onb);
}

// これ作る
// t_vec3 local_onb(t_onb onb, t_vec3 vec)
