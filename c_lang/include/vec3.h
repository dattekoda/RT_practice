#ifndef VEC3_H
# define VEC3_H

typedef struct s_vec3
{
	double	x;
	double	y;
	double	z;
}	t_vec3;

t_vec3	construct_vec(double _x, double _y, double _z);
t_vec3	negative_vec(t_vec3 vec);
t_vec3	add_vec(t_vec3 lhs, t_vec3 rhs);
t_vec3	sub_vec(t_vec3 lhs, t_vec3 rhs);
t_vec3	mul_vec(t_vec3 lhs, t_vec3 rhs);
t_vec3	scal_mul_vec(t_vec3 lhs, double rhs);
double	length_squared_vec(t_vec3 vec);
double	length_vec(t_vec3 vec);
double	dot(t_vec3 lhs, t_vec3 rhs);
t_vec3	cross(t_vec3 lhs, t_vec3 rhs);
t_vec3	normalize(t_vec3 vec);
double	axis_vec(t_vec3 vec, int axis);
// t_vec3	random_vec(double min, double max);
t_vec3	constant_vec(double d);
t_vec3	map_vec(t_vec3 vec, double (*f)(double));

t_vec3	random_unit_vector(void);

t_vec3	get_vec(char **line);


void	check_vec(t_vec3 vec);


typedef t_vec3	t_point3;

#endif