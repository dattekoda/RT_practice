#include "color.h"
#include <stddef.h>

int	get_raw_rgb(t_color pixel_color)
{
	int	int_color[3];
	double	double_color[3];
	size_t	i;

	i = 0;
	while (i < 3)
	{
		double_color[i] = sqrt(axis_vec(pixel_color, i));
	}
	// ガンマ補正
	double_color[0] = sqrt(pixel_color.x);
	double_color[1] = sqrt(pixel_color.y);
	double_color[2] = sqrt(pixel_color.z);
}