#include "img.h"
#include "define.h"

typedef struct s_hook_var
{
	void	*mlx;
	void	*win;
}	t_hook_var;

void	draw(void **mlx, t_img *img);

void	initialize(t_hook_var *var, t_img *img)
{
	var->mlx = mlx_init();
	var->win = mlx_new_window(var->mlx, WINSIZE_X, WINSIZE_Y, "miniRT");
	construct_img(img, var->mlx);
	return ;
}

int	main(int argc, char *argv[])
{
	t_hook_var	var;
	t_img		img;

	if (argc == 1)
		return (perror("input err"), 1);
	if (validate(argv[1]))
		return (perror("validate err"), 1);
	initialize(&var, &img);
	draw(&var.mlx, &img);
	return (0);
}