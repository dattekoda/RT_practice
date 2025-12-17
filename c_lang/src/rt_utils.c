#include "libft.h"
#include <stdlib.h>

static uint64_t	random_uint64(void)
{
	static uint64_t	x = 0x629d97afad9281f2;

	x ^= x << 12;
	x ^= x >> 25;
	x ^= x << 27;
	return (x);
}

double	random_double(double min, double max)
{
	uint64_t	u;
	double		d;

	u = random_uint64() >> 11; // 仮数部取得
	d = (double)u * (1.0 / 9007199254740992.0); // 2^53で割って0-1に正規化
	return (min + (max - min) * d); 
}

/*
@brief min <= maxであることに注意
*/
int	random_int(int min, int max)
{
	uint64_t	u;

	u = random_uint64();
	u = u % (max - min + 1);
	return (min + u);
}

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

// #include <stdio.h>

// int	main(int c, char *v[])
// {
// 	if (c != 3)
// 		return 1;
// 	int	min = atoi(v[1]);
// 	int	max = atoi(v[2]);
// 	for (int i = 0; i < 100; i++)
// 		fprintf(stderr, "%d\n", random_int(min, max));
// }