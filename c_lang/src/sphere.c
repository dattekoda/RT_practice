#include "sphere.h"
#include "rt_utils.h"
#include "discriminant.h"
#include "t_range.h"
#include <math.h>
#include <stdlib.h>

static bool	bounding_sphere(const t_sphere *self, t_range range, t_aabb *output_box)
{
	(void)range;
	*output_box = construct_aabb(sub_vec(self->center, constant_vec(self->radius)), \
							add_vec(self->center, constant_vec(self->radius)));
	return (true);
}

static t_abc	calc_abc(const t_sphere *self, const t_ray ray)
{
	t_abc	abc;
	t_vec3	oc;

	oc = sub_vec(ray.origin, self->center);
	abc.x = length_squared_vec(ray.direct);
	abc.y = dot(oc, ray.direct);
	abc.z = dot(oc, oc) - pow2(self->radius);
	return (abc);
}

static bool	set_sphere_hitrec(const t_sphere *self, const t_ray ray, t_hit_record *rec, double solution)
{
	rec->ray_in = ray;
	rec->t = solution;
	rec->point = ray.at(&ray, rec->t);
	rec->normal = scal_mul_vec(sub_vec(rec->point, self->center), 1 / self->radius);
	rec->mat_ptr = self->hit_table.mat_ptr;
	return (true);
}

static bool	hit_sphere(const void *s, const t_ray ray, t_hit_record *rec, t_t_range t_range)
{
	const t_sphere	*self;
	t_abc			abc;
	double			discriminant;
	double			root;
	double			solution;

	self = (const t_sphere *)s;
	abc = calc_abc(self, ray);
	discriminant = calc_discriminant(abc);
	if (0 < discriminant)
	{
		root = sqrt(discriminant);
		solution = (-abc.y - root) / abc.x;
		if (check_range(solution, t_range))
			return (set_sphere_hitrec(self, ray, rec, solution));
		solution = (-abc.y + root) / abc.x;
		if (check_range(solution, t_range))
			return (set_sphere_hitrec(self, ray, rec, solution));
	}
	return (false);
}

t_sphere	construct_sphere(t_point3 _cen, double _rad, void *mat_ptr)
{
	t_sphere	sphere;

	sphere.center = _cen;
	sphere.radius = _rad;
	sphere.hit_table.mat_ptr = mat_ptr;
	sphere.hit_table.hit = hit_sphere;
	sphere.hit_table.bounding_box = bounding_sphere;
	return (sphere);
}

t_sphere	*gen_sphere(t_point3 _cen, double _rad, void *mat_ptr)
{
	t_sphere	*sphere;

	sphere = calloc(1, sizeof(t_sphere));
	if (!sphere)
		return (NULL);
	*sphere = construct_sphere(_cen, _rad, mat_ptr);
	return (sphere);
}