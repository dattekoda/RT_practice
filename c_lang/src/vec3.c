#include "vec3.h"
#include "rt_utils.h"
#include <math.h>
#include <stddef.h>

t_vec3	construct_vec(double _x, double _y, double _z)
{
	t_vec3	vec;

	vec.x = _x;
	vec.y = _y;
	vec.z = _z;
	return (vec);
}

t_vec3	negative_vec(t_vec3 vec)
{
	return (construct_vec(-vec.x, -vec.y, -vec.z));
}

t_vec3	add_vec(t_vec3 lhs, t_vec3 rhs)
{
	return (construct_vec(
		lhs.x + rhs.x, \
		lhs.y + rhs.y, \
		lhs.z + rhs.z \
	));
}

t_vec3	sub_vec(t_vec3 lhs, t_vec3 rhs)
{
	return (construct_vec(
		lhs.x - (rhs.x), \
		lhs.y - (rhs.y), \
		lhs.z - (rhs.z) \
	));
}

t_vec3	mul_vec(t_vec3 lhs, t_vec3 rhs)
{
	return (construct_vec(
		(lhs.x) * (rhs.x), \
		(lhs.y) * (rhs.y), \
		(lhs.z) * (rhs.z) \
	));
}

t_vec3	scal_mul_vec(double lhs, t_vec3 rhs)
{
	return (construct_vec(
		(lhs) * (rhs.x), \
		(lhs) * (rhs.y), \
		(lhs) * (rhs.z) \
	));
}

double	length_squared_vec(t_vec3 vec)
{
	return (dot(vec, vec));
}

double	length_vec(t_vec3 vec)
{
	return (sqrt(length_squared_vec(vec)));
}

double	dot(t_vec3 lhs, t_vec3 rhs)
{
	return (lhs.x * rhs.x + \
		lhs.y * rhs.y + \
		lhs.z * rhs.z);
}

t_vec3	cross(t_vec3 lhs, t_vec3 rhs)
{
	return (construct_vec(
		lhs.y * rhs.z - lhs.z * rhs.y, \
		lhs.z * rhs.x - lhs.x * rhs.z, \
		lhs.x * rhs.y - lhs.y * rhs.x \
	));
}

t_vec3	normalize(t_vec3 vec)
{
	return (scal_mul_vec(1 / length_vec(vec), vec));
}

double	*axis_vec(t_vec3 *vec, int axis)
{
	if (axis == 0)
		return (&vec->x);
	if (axis == 1)
		return (&vec->y);
	if (axis == 2)
		return (&vec->z);
	return (NULL);
}

t_vec3	constant_vec(double d)
{
	return (construct_vec(d, d, d));
}

t_vec3	map_vec(t_vec3 vec, double (*f)(double))
{
	vec.x = f(vec.x);
	vec.y = f(vec.y);
	vec.z = f(vec.z);
	return (vec);
}
