#ifndef HIT_RECORD_H
# define HIT_RECORD_H

# include "ray.h"
# include "vec3.h"

typedef struct s_material t_material;

/*
@brief hitした情報を保存する構造体
@param ray_in 入射したときのレイの情報
@param point ヒットした点
@param normal ヒットした点での法線ベクトル
@param mat_ptr マテリアルの情報
@param t 入射したレイのt情報
*/
typedef struct s_hit_record
{
	t_ray		ray_in;
	t_point3	point;
	t_vec3		normal;
	t_material	*mat_ptr;
	double		t;
}	t_hit_record;

#endif