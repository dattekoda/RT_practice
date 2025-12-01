#ifndef HITTABLE_H
# define HITTABLE_H

# include "ray.h"
# include <stdbool.h>

typedef struct s_hit_rec
{
	t_vec	point;
	t_vec	normal;
	double	t;
	bool	front_face;
	void	(*set_face_normal)(t_hit_rec*,t_ray*,t_vec*);
}	t_hit_rec;

#endif
