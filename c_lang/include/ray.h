#ifndef RAY_H
# define RAY_H

# include "vec3.h"
# include "camera.h"

typedef struct s_ray t_ray;

struct s_ray
{
	t_point3	origin;
	t_vec3		direct;
	t_point3	(*at)(const t_ray *self, double t);
};

t_ray		construct_ray(t_point3 _origin, t_vec3 _direct);
t_ray		get_ray(const t_camera *camera, double u, double v);

#endif