#include "img.h"
#include "mlx.h"
#include "world.h"

void	draw(void **mlx, t_img *img)
{
	t_world		world = set_world();
	t_camera	camera = construct_camera(construct_vec(0, 1, 0), normalize(construct_vec(0, -0.3, -1)), 70);
}