#include "img.h"
#include "mlx.h"
#include "world.h"
#include "camera.h"
#include "define.h"
#include "rt_utils.h"
#include "mlx.h"
#include "libft.h"
#include <stdio.h>
#include <math.h>

int				close_window(void *param);
void			write_loop(void **mlx, t_img *img, t_color *color_arr);
static void		pixel_color_loop(t_color *color_arr, const t_world *world);
static t_color	accumulate_pixel_color(int x, int y, const t_world *world);
static bool		killed_by_russian_roulette(t_color *attenuation);
static t_color	ray_color(t_ray ray, const t_world *world, int depth);

void	draw(const t_world *world)
{
	t_hook_var	var;
	t_img		img;
	t_color		*color_arr = ft_calloc(WINSIZE_X * WINSIZE_Y, sizeof(t_color));

	initialize(&var, &img);
	pixel_color_loop(color_arr, world);
	write_loop(&var.mlx, &img, color_arr);
	fprintf(stderr, "\nDone.\n");
	free(color_arr);
	mlx_put_image_to_window(var.mlx, var.win, img.id, 0, 0);
	mlx_hook(var.win, 17, 0L, close_window, &var);
	mlx_loop(var.mlx);
}

static void	pixel_color_loop(t_color *color_arr, const t_world *world)
{
	int	x;
	int	y;
	int	yy;

	y = 0;
	while (y < WINSIZE_Y)
	{
		yy = y * WINSIZE_X;
		fprintf(stderr, "\rScanlines remaining: %d ", WINSIZE_Y - y - 1);
		x = 0;
		while (x < WINSIZE_X)
		{
			color_arr[yy + x] = accumulate_pixel_color(x, y, world);
			x++;
		}
		y++;
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
	while (s++ < SAMPLES_PER_PIXEL)
	{
		u = (x + random_double(0, 1)) / (WINSIZE_X - 1);
		v = (y + random_double(0, 1)) / (WINSIZE_Y - 1);
		ray = get_ray(&world->camera, u, v);
		pixel_color = add_vec(pixel_color, ray_color(ray, world, 0));
	}
	pixel_color = scal_mul_vec(pixel_color, scale);
	return (pixel_color);
}

static t_color	ray_color(t_ray ray, const t_world *world, int depth)
{
	t_hit_record	rec;
	t_range			init_range;

	init_range = construct_range(HIT_T_MIN, INFINITY);
	if (MAX_DEPTH <= depth)
		return (construct_color(0, 0, 0));
	if (world->object_tree == NULL \
		|| check_bvh(world->object_tree, ray, &rec, init_range) == false)
		return (world->back_ground);
	t_ray			scattered;
	t_color			attenuation; // 減衰
	t_color			emitted;

	emitted = rec.mat_ptr->emitted(rec.mat_ptr, rec);
	if (rec.mat_ptr->scatter(rec.mat_ptr, rec, &attenuation, &scattered) == false)
		return (emitted);
	if (RR_START_DEPTH < depth && killed_by_russian_roulette(&attenuation))
		return (emitted);
	return (add_vec(emitted, mul_vec(attenuation, ray_color(scattered, world, depth + 1))));
}

static bool	killed_by_russian_roulette(t_color *attenuation)
{
	double	live_prob;

	live_prob = fmax(fmax(attenuation->x, attenuation->y), attenuation->z);
	if (live_prob <= random_double(0, 1))
		return (true);
	*attenuation = scal_mul_vec(*attenuation, 1 / live_prob);
	return (false);
}

void	write_loop(void **mlx, t_img *img, t_color *color_arr)
{
	int		y;
	int		x;
	int		yy;
	char	*dst;

	y = 0;
	while (y < WINSIZE_Y)
	{
		yy = y * WINSIZE_X;
		x = 0;
		while (x < WINSIZE_X)
		{
			dst = img->addr + (y * img->line_size + x * img->bytes_per_pixels);
			write_color(*mlx, dst, color_arr[yy + x++]);
		}
		y++;
	}
}

int	close_window(void *param)
{
	t_hook_var	*vars;

	vars = (t_hook_var *)param;
	if (vars->win)
		mlx_destroy_window(vars->mlx, vars->win);
	exit(0);
	return (0);
}