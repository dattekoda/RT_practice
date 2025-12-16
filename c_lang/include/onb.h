#ifndef ONB_H
# define ONB_H

# include "vec3.h"

/*
@brief 正規直行基底 OrthoNormal Basis
@details 各ベクトルは長さ1かつそれぞれが直行している.
@param u 親指
@param v 人差し指
@param w 中指
*/
typedef struct s_onb
{
	t_vec3	u;
	t_vec3	v;
	t_vec3	w;
}	t_onb;

t_onb	construct_w_onb(t_vec3 _w, t_vec3 upper);

#endif