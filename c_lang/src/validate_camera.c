#include "rt_utils.h"
#include "libft.h"
#include <stdlib.h>

// You need to imprement new function
// which return true if two doubles are almost equal
// but in the first place, libft doesn't
// have fabs nor strtof so...
int	validate_camera(char *line_ptr)
{
	double	d_tmp;
	size_t	i;

	line_ptr++;
	i = 0;
	while (i++ < 3)
	{
		d_tmp = ft_strtod(line_ptr, &line_ptr);
		if (i != 3 && *(line_ptr++) != ',')
			return (EXIT_FAILURE);
	}
	if (skip_spaces(&line_ptr))
		return (EXIT_FAILURE);
	i = 0;
	d_tmp = 0;
	while (i++ < 3)
	{
		d_tmp += pow2(ft_strtod(line_ptr, &line_ptr));
		if (i != 3 && *(line_ptr++) != ',')
			return (EXIT_FAILURE);
	}
	if (0.001 < d_tmp - 1)
		return (EXIT_FAILURE);
	if (skip_spaces(&line_ptr))
		return (EXIT_FAILURE);
	d_tmp = ft_strtod(line_ptr, &line_ptr);
	if (d_tmp < 0.0f || 180.0f < d_tmp)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
