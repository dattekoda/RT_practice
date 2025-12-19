#include "img.h"
#include "define.h"
#include "read_rt.h"
#include "validate.h"
#include "world.h"
#include <stdio.h>
#include <stdlib.h>

void	draw(t_world *world);
int		set_world(t_world *world, t_list *line_lst);


void	print_world(const t_world *world);
int	main(int argc, char *argv[])
{
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
	// print_world(&world);
	ft_lstclear(&line_lst, free);
	draw(&world);
	return (0);
}
