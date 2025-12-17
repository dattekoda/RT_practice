#include "img.h"
#include "define.h"
#include "read_rt.h"
#include "validate.h"
#include "world.h"
#include <stdio.h>

typedef struct s_hook_var
{
	void	*mlx;
	void	*win;
}	t_hook_var;

void	draw(void **mlx, t_img *img);
int		set_world(t_world *world, t_list *line_lst);

// void	initialize(t_hook_var *var, t_img *img)
// {
// 	var->mlx = mlx_init();
// 	var->win = mlx_new_window(var->mlx, WINSIZE_X, WINSIZE_Y, "miniRT");
// 	construct_img(img, var->mlx);
// 	return ;
// }

int	main(int argc, char *argv[])
{
	t_hook_var	var;
	t_img		img;
	t_list		*line_lst;
	t_world		world;

	if (argc == 1)
		return (perror("input err"), EXIT_FAILURE);
	if (read_rt(&line_lst, argv[1]))
		return (EXIT_FAILURE);
	if (validate(line_lst))
		return (EXIT_FAILURE);
	if (set_world(&world, line_lst))
		return (EXIT_FAILURE);
	// initialize(&var, &img);
	// draw(&var.mlx, &img);
	return (0);
}