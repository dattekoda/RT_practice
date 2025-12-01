/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khanadat <khanadat@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 15:45:11 by khanadat          #+#    #+#             */
/*   Updated: 2025/12/01 22:07:46 by khanadat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "c_rt.h"
#include "ray.h"
#include <stdio.h>

t_vec	ray_color(const t_ray *r)
{
	t_vec	unit_dir;
	t_vec	left;
	t_vec	right;
	double	t;

	unit_dir = vec_unit(&r->dir);
	t = 0.5 * (unit_dir.y + 1.0);
	left = vec_set_vec(1.0, 1.0, 1.0);
	left = vec_mul_double_vec(1.0 - t, &left);
	right = vec_set_vec(0.5, 0.7, 1.0);
	right = vec_mul_double_vec(t, &right);
	return vec_add(&left, &right);
}

void	print_color(int i, int j, int image_width, int image_height)
{
	double	r;
	double	g;
	double	b;

	r = (double)i / (double)(image_width - 1);
	g = (double)j / (double)(image_height - 1);
	b = 0.25;
	printf("%d %d %d\n", (int)(255.999 * r), (int)(255.999 * g), (int)(255.999 * b));
}

int	main(void)
{
	const double	aspect_ratio = 16.0 / 9.0;
	const int		image_width = 384;
	const int		image_height = (int)((double)image_width / aspect_ratio);

	size_t		i = 0;
	size_t		j = image_height - 1;

	printf("P3\n%d %d\n255\n",image_width, image_height);

	double	viewport_height = 2.0;
	double	viewport_width = aspect_ratio * viewport_height;
	double	focal_length = 1.0;
	
	t_vec	origin = vec_set_vec(0.0, 0.0, 0.0);
	t_vec	horizontal = vec_set_vec(viewport_height, 0, 0);
	t_vec	vertical = vec_set_vec(0, viewport_height, 0);
	t_vec	lower_left_corner = vec_set_vec(0.0, 0.0, 0.0);
	lower_left_corner.add(&lower_left_corner, &horizontal);
	lower_left_corner.add(&lower_left_corner, &vertical);
	t_vec	divide = vec_set_double(1 / 2);
	lower_left_corner.mul(&lower_left_corner, &divide);
	t_vec	focal_vec = vec_set_vec(0.0, 0.0, focal_length);
	lower_left_corner.add(&lower_left_corner, &focal_vec);
	lower_left_corner.minus(&lower_left_corner);
	lower_left_corner.add(&lower_left_corner, &origin);
	while (0 <= j)
	{
		while (i < image_width)
		{
			t_vec	u = vec_set_double((double)i / (double)(image_width));
			t_vec	v = vec_set_double((double)j / (double)(image_height));

			t_vec	_dir = lower_left_corner;
			t_vec	_horizontal = vec_mul(&u, &horizontal);
			t_vec	_vertical = vec_mul(&v, &vertical);
			_dir.add(&_dir, &_horizontal);
			_dir.add(&_dir, &_vertical);
			t_ray	r = ray_set();
		}
			print_color(i, j, image_width, image_height);
	}
	return (0);
}
