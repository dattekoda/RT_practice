#ifndef LAMBERTIAN_H
# define LAMBERTIAN_H

# include "material.h"

typedef struct	s_lambertian
{
	t_material	material;
	t_color		albedo;
}	t_lambertian;

t_lambertian	*gen_lambertian(t_color _albedo);

#endif