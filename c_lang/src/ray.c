#include "ray.h"
#include "camera.h"

/*
@brief 引数tを元に座標を返す関数
*/
t_point3	at_ray(const t_ray *self, double t)
{
	return (add_vec(scal_mul_vec(self->direct, t), self->origin));
}

t_ray	construct_ray(t_point3 _origin, t_vec3 _direct)
{
	t_ray	ray;

	ray.origin = _origin;
	ray.direct = _direct;
	ray.at = at_ray;
	return (ray);
}

// 配列へ保存しておくようにしたらもっと早くなるかも?
t_ray	get_ray(const t_camera *camera, double u, double v)
{
	return (construct_ray(
		camera->origin, \
			sub_vec( \
				add_vec( \
					add_vec( \
						scal_mul_vec(camera->horizontal, u), \
						scal_mul_vec(camera->vertical, v)), \
					camera->higher_left_corner), \
				camera->origin
	)));
	// u*hori + v*vert + higher_left_corner - origin
}

// test case
// cc ray.c vec3.c -I ../include -lm

// #include <stdlib.h>
// #include <stdio.h>
// int	main(int argc, char *argv[])
// {
// 	t_ray	ray;

// 	if (argc == 1)
// 		return 1;
// 	ray = construct_ray(constant_vec(0), \
// 	construct_vec(2.0f, 1.0f, -1.0f));
// 	t_point3	at_point = ray.at(&ray, atof(argv[1]));
// 	fprintf(stderr, "%f, %f, %f\n", at_point.x, at_point.y, at_point.z);
// 	return (0);
// }
