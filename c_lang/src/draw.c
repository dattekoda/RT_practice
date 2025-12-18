#include "img.h"
#include "mlx.h"
#include "world.h"
#include "camera.h"
#include "define.h"
#include "rt_utils.h"
#include "mlx.h"
#include "libft.h"
#include <stdio.h>

void	initialize(t_hook_var *var, t_img *img);

void	draw(t_world *world)
{
	t_hook_var	var;
	t_img		img;
	int			x;
	int			y;
	int			yy;
	t_color		*color_arr = ft_calloc(WINSIZE_X * WINSIZE_Y, sizeof(t_color));

	initialize(&var, &img);
	y = 0;
	while (y < WINSIZE_Y)
	{
		yy = y * WINSIZE_X;
		fprintf(stderr, "\rScanlines remaining: %d ", WINSIZE_Y - y - 1);
		x = 0;
		while (x < WINSIZE_X)
			color_arr[yy + x] = accumulate_pixel_color(x, y, &world->camera, world->object_tree);
	}
}

static t_color	accumulate_pixel_color(int x, int y, const t_world *world)
{
	static const double	scale = 1.0f / SAMPLES_PER_PIXEL;
	t_color				pixel_color = construct_color(0,0,0);
	int					s;
	double				u;
	double				v;
	t_ray				ray;

	s = 0;
	while (s < SAMPLES_PER_PIXEL)
	{
		u = (x + random_double(0, 1)) / (WINSIZE_X - 1);
		v = (y + random_double(0, 1)) / (WINSIZE_Y - 1);
		ray = get_ray(&world->camera, u, v);
		pixel_color = add_vec(pixel_color, ray_color(ray, world, 0));
	}
	pixel_color = scal_mul_vec(pixel_color, scale);
	return (pixel_color);
}

t_color	ray_color(t_ray ray, const t_world *world, int depth)
{
	t_hit_record	rec;

	if (MAX_DEPTH <= depth)
		return (construct_color(0, 0, 0));
// 未実装!
}