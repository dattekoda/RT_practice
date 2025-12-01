#ifndef RAY_H
# define RAY_H

# include "vec3.h"

typedef struct	s_ray
{
	t_vec	orig;
	t_vec	dir;
	void	(*set_unit)(t_vec*, t_vec*);
}	t_ray;

void	set_unit(t_vec *self, t_vec *_cpy);
t_ray	ray_init(void);
t_ray	ray_set(t_vec *_orig, t_vec *_dir);
t_vec	ray_at(const t_ray *ray, const double t);

#endif
