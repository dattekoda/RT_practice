#include "libft.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

int	validate_ambient(char *line);

int	validate_distribute(char *line)
{
	static const char	*object_identifier[] = {
		"sp", "pl", "cy", NULL
	};
	size_t				i;

	if (*line == 'A')
		return (validate_ambient(line));
	if (*line == 'C')
		return (validate_camera(line));
	if (*line == 'L')
		return (EXIT_SUCCESS);
	i = 0;
	while (object_identifier[i])
	{
		if (!ft_strncmp(line, object_identifier[i++], 2))
			return (EXIT_SUCCESS);
	}
	return (EXIT_FAILURE);
}

int	validate_content(t_list *line_lst)
{
	char	*line_ptr;

	while (line_lst)
	{
		if (validate_line_ptr(line_lst->content))
			return (EXIT_FAILURE);
		line_lst = line_lst->next;
	}
	return (EXIT_SUCCESS);
}
