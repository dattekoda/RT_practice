#ifndef HIT_TABLE_H
# define HIT_TABLE_H

# include "hit_record.h"
# include "ray.h"
# include "material.h"
# include "aabb.h"
# include <stdbool.h>

/*
@brief オブジェクトを表す一般的なクラス
@param bounding_box 包含直方体が作成できたかどうか
*/
typedef struct s_hit_table
{
	bool		(*bounding_box)(const void *s, t_range range, t_aabb *output_box);
	bool		(*hit)(const void *, const t_ray, t_hit_record *, t_range);
	t_material	*mat_ptr;
}	t_hit_table;

#endif