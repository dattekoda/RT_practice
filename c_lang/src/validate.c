#include "libft.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

int	validate_ambient(char *line);
int	validate_camera(char *line);
int	validate_sphere(char *line);

static int	validate_distribute(char *line)
{
	static const char	*object_identifier[] = {
		"sp",/*  "pl", "cy", */ NULL
	};
	size_t				i;

	if (*line == 'A')
		return (validate_ambient(line + 1));
	if (*line == 'C')
		return (validate_camera(line + 1));
	// if (*line == 'L')
	// 	return (EXIT_SUCCESS);
	if (ft_strncmp(line, "sp", 2) == 0)
		return (validate_sphere(line + 2));
	return (EXIT_FAILURE);
}

int	validate(t_list *line_lst)
{
	char	*line_ptr;

	while (line_lst)
	{
		if (validate_distribute(line_lst->content))
			return (EXIT_FAILURE);
		line_lst = line_lst->next;
	}
	return (EXIT_SUCCESS);
}
