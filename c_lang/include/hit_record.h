#ifndef HIT_RECORD_H
# define HIT_RECORD_H

# include "ray.h"
# include "vec3.h"
# include "material.h"

typedef struct s_hit_record
{
	t_ray		ray_in;
	t_point3	point;
	t_vec3		normal;
	t_material	*mat_ptr;
	double		t;
}	t_hit_record;

#endif