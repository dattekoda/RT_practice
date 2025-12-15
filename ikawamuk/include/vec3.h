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
t_vec3	scal_mul_vec(double lhs, t_vec3 rhs);
double	length_squared_vec(t_vec3 vec);
double	length_vec(t_vec3 vec);
double	dot(t_vec3 lhs, t_vec3 rhs);
t_vec3	cross(t_vec3 lhs, t_vec3 rhs);
t_vec3	normalize(t_vec3 vec);
double	axis_vec(t_vec3 vec, int axis);
// t_vec3	random_vec(double min, double max);

typedef t_vec3	t_point3;

#endif