#include "ray.h"
#include "hittable.h"
#include "libft.h"

static void	set_face_normal(const t_hit_rec *self, const t_ray *ray, const t_vec *outward_normal)
{
	t_vec	_outward_normal;

	self->front_face = vec_dot(&ray->dir, outward_normal) < 0;
	if (self->front_face)
		self->normal.cpy(&self->normal, outward_normal);
	else
	{
		_outward_normal = outward_normal->minus(outward_normal);
		self->normal.cpy(&self->normal, &_outward_normal);
	}
}

t_hit_rec	hit_rec_set(void)
{
	t_hit_rec	res;

	ft_bzero(&res, sizeof(t_hit_rec));
	res.normal = vec_init();
	res.point = vec_init();
	return (res);
}
