#include "hit_node.h"

#include <stdio.h>
#include <stdlib.h>
/*
@brief bvh木構造をレイとの交差判定を行う
@param 
*/
bool	check_bvh(const t_hit_node *self, t_ray ray, t_hit_record *rec, t_range range)
{
	bool	hit_left;
	bool	hit_right;

	if (self == NULL)
		return (false);
	// まずboxにヒットしないものをfalseとする
	if (self->box.hit(&self->box, ray, range) == false)
		return (false);
	// dataがある時はそれがノードではなく葉であることを示す
	if (self->data)
		return (self->data->hit(self->data, ray, rec, range));
	hit_left = check_bvh(self->lhs, ray, rec, range);
	if (hit_left)
		range.max = rec->t;
	hit_right = check_bvh(self->rhs, ray, rec, range);
	return (hit_left || hit_right);
}
