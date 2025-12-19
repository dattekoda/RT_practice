#include "hit_node.h"

#include <stdio.h>
#include <stdlib.h>
/*
@brief bvh木構造をレイとの交差判定を行う
@param 
*/
bool	check_bvh(const t_hit_node *self, t_ray ray, t_hit_record *rec, t_range range)
{
	t_hit_record		tmp_rec;
	bool				hit_anything;

	if (self == NULL)
		return (false);
	hit_anything = false;
	// まずboxにヒットしないものをfalseとする
	if (self->box.hit(&self->box, ray, range) == false)
		return (false);
	// dataがある時はそれがノードではなく葉であることを示す
	if (self->data)
	{
		// boxにhitしたら物体自体にhitするかどうか判別したい
		if (self->data->hit(self->data, ray, &tmp_rec, range))
		{
			*rec = tmp_rec;
			return (true);
		}
	}
	else
	{
		if (check_bvh(self->lhs, ray, &tmp_rec, range))
		{
			*rec = tmp_rec;
			hit_anything = true;
		}
		if (check_bvh(self->lhs, ray, &tmp_rec, range))
		{
			*rec = tmp_rec;
			hit_anything = true;
		}
	}
	return (hit_anything);
}
