#include "camera.h"
#include "define.h"
#include "onb.h"
#include <math.h>

static double	degrees_to_radians(const double degrees)
{
	double	deno;

	deno = 1.0f / 180.0f;
	return (degrees * M_PI * deno);
}

/*
@param _direct 向きベクトル, 正規化不要
@param vfov 縦方向視野角
*/
t_camera	construct_camera(t_point3 _origin, t_vec3 _direct, double vfov)
{
	t_camera		camera;
	double			theta = degrees_to_radians(vfov * 0.5);
	double			h = tan(theta);
	double			screen_h = 2.0f * h;
	double			screen_w = screen_h * ASPECT_RATIO;
	t_onb			onb;

	onb = construct_w_onb(negative_vec(_direct));
	camera.origin = _origin;
	camera.horizontal = scal_mul_vec(onb.u, screen_w);
	camera.vertical = scal_mul_vec(onb.v, -screen_h);
	camera.higher_left_corner = \
	add_vec( \
		sub_vec(\
			negative_vec(onb.w), \
			scal_mul_vec( \
				add_vec(camera.horizontal, camera.vertical), \
				0.5)), \
		camera.origin);
	// -w -0.5 (horizontal + vertical) + origin
	return (camera);
}
