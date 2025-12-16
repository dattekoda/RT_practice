#ifndef SPHERE_H
# define SPHERE_H

# include "hit_table.h"
# include "vec3.h"

typedef struct s_sphere
{
	t_hit_table	hit_table;
	t_point3	center;
	double		radius;
}	t_sphere;

t_sphere	*gen_sphere(t_point3 _cen, double _rad, void *mat_ptr);

#endif