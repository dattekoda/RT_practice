#include "ray.h"
#include "libft.h"

// void	set_unit(t_vec *self, const t_vec _cpy)
// {
// 	self->cpy(_cpy);
// }

t_ray	ray_init(void)
{
	t_ray	res;

	ft_bzero(&res, sizeof(t_ray));
	res.set_unit = &set_unit;
	res.dir = vec_init();
	res.orig = vec_init();
	return (res);
}

t_ray	ray_set(t_vec *_orig, t_vec *_dir)
{
	t_ray	ray;

	ray = ray_init();
	ray.set_unit(&ray.orig, _orig);
	ray.set_unit(&ray.dir, _dir);
	return (ray);
}

t_vec	ray_at(const t_ray *ray, const double t)
{
	t_vec	_double;
	t_vec	_mul;

	_double = vec_set_double(t);
	_mul = vec_mul(&ray->dir, &_double);
	return (vec_add(&ray->dir, &_mul));
}
