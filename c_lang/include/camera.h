#ifndef CAMERA_H
# define CAMERA_H

# include "vec3.h"

/*
@param horizontal 横軸方向の単位ベクトル originから見て右方向
@param vertical 縦軸方向の単位ベクトル originから見て下方向
*/
typedef struct s_camera
{
	t_point3	origin;
	t_point3	higher_left_corner;
	t_vec3		horizontal;
	t_vec3		vertical;
}	t_camera;

t_camera	construct_camera(t_point3 _origin, t_vec3 _direct, double vfov);

#endif