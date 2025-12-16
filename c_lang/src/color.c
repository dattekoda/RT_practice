#include "color.h"
#include "rt_utils.h"
#include <stddef.h>
#include <math.h>

static double	get_raw_rgb_clamp(double x)
{
	return (256 * clamp(x, 0.0f, 0.999f));
}

int	get_raw_rgb(t_color pixel_color)
{
	pixel_color = map_vec(pixel_color, &sqrt);
	pixel_color = map_vec(pixel_color, &get_raw_rgb_clamp);
	return ((int)pixel_color.x << 16 | (int)pixel_color.y << 8 | (int)pixel_color.z);
}

static double color_clamp(double x)
{
	return (clamp(x, 0.0f, 1.0f));
}

t_color	construct_color(double r, double g, double b)
{
	return (map_vec(construct_vec(r, g, b), &color_clamp));
}

// cc color.c rt_utils.c vec3.c -I ../include -lm

// #include <stdio.h>
// int	main(void)
// {
// 	t_color	p = construct_vec(0.5f, 0.6f, 0.5f);
// 	int	color = get_raw_rgb(p);
// 	fprintf(stderr, "%d\n", color >> 16 & 0x000000ff);
// 	fprintf(stderr, "%d\n", color >> 8 & 0x000000ff);
// 	fprintf(stderr, "%d\n", color & 0x000000ff);
// }
