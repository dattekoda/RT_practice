#include "libft.h"
#include <stdlib.h>

double	clamp(double x, double min, double max)
{
	if (x < min)
		return (min);
	if (max < x)
		return (max);
	return (x);
}

double	pow2(double a)
{
	return ((a) * (a));
}

/*
@brief spaceがなかった時false*/
int	skip_spaces(char **ptr)
{
	if (**ptr != ' ' && **ptr != '\t')
		return (EXIT_FAILURE);
	while (**ptr == ' ' || **ptr == '\t')
		(*ptr)++;
	return (EXIT_SUCCESS);
}

int	skip_digit(char **ptr)
{
	if (**ptr == '-' || **ptr == '+')
		(*ptr)++;
	if (!ft_isdigit(**ptr))
		return (EXIT_FAILURE);
	while (ft_isdigit(**ptr))
		(*ptr)++;
	return (EXIT_SUCCESS);
}
