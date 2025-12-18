#include "img.h"
#include "define.h"
#include "mlx.h"

void	construct_img(t_img *img, void *mlx)
{
	img->id = mlx_new_image(mlx, WINSIZE_X, WINSIZE_Y);
	img->addr = mlx_get_data_addr(
		img->id, \
		&img->bytes_per_pixels, \
		&img->line_size, \
		&img->endian \
	);
	img->bytes_per_pixels /= 8;
}

void	initialize(t_hook_var *var, t_img *img)
{
	var->mlx = mlx_init();
	var->win = mlx_new_window(var->mlx, WINSIZE_X, WINSIZE_Y, "miniRT");
	construct_img(img, var->mlx);
	return ;
}