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
	// カメラの方向ベクトルが正規化されているか
	// 縦方向の視野角が0-180の範囲にあるか
}
