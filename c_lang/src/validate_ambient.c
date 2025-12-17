#include "rt_utils.h"
#include "libft.h"
#include <stddef.h>
#include <stdlib.h>

//necessary to implement strtod
static void	err_ambient(void)
{
	static char	*msg = \
"\x1b[31m""Set ambient information correctly.\n" \
"\x1b[0m""A [lighting ratio] ""\x1b[31m""R"",""\x1b[32m""G"",""\x1b[34m""B";

	ft_putendl_fd(msg, STDERR_FILENO);
}

int	validate_ambient(char *line)
{
	double	lighting_ratio;
	int		tmp;
	size_t	i;

	if (skip_spaces(&line))
		return (err_ambient(), EXIT_FAILURE);
	lighting_ratio = ft_strtod(line, &line);
	if (lighting_ratio < 0 || 1 < lighting_ratio)
		return (err_ambient(), EXIT_FAILURE);
	if (skip_spaces(&line))
		return (err_ambient(), EXIT_FAILURE);
	i = 0;
	while (i++ < 3)
	{
		tmp = ft_atoi(line);
		if ((tmp < 0 || 255 < tmp) || skip_digit(&line))
			return (err_ambient(), EXIT_FAILURE);
		if (i != 3 && *(line++) != ',')
			return (err_ambient(), EXIT_FAILURE);
	}
	if (*line == '\n')
		return (EXIT_SUCCESS);
	return (err_ambient(), EXIT_FAILURE);
}
