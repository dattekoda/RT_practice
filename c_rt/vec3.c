#include "vec3.h"
#include "libft.h"
#include <math.h>

static void	set_cpy(t_vec *self, const t_vec _cpy)
{
	ft_memmove(self, &_cpy, sizeof(t_vec));
}

static double	set_length_squared(const t_vec *self)
{
	return ((self->x) * (self->x) + \
		(self->y) * (self->y) + \
		(self->z) * (self->z));
}

static double	set_length(const t_vec *self)
{
	return (sqrt(self->length_squared(self)));
}

static t_vec	set_minus(const t_vec *self)
{
	t_vec	res;

	res = vec_init();
	res.cpy(&res, *self);
	res.x *= (-1);
	res.y *= (-1);
	res.z *= (-1);
	return (res);
}

static void	set_add(t_vec *self, const t_vec u)
{
	self->x += u.x;
	self->y += u.y;
	self->z += u.z;
}

static void	set_subt(t_vec *self, const t_vec u)
{
	self->x -= u.x;
	self->y -= u.y;
	self->z -= u.z;
}

static void	set_mul(t_vec *self, const t_vec u)
{
	if (self->type == T_VEC && u.type == T_VEC)
	{
		self->x *= u.x;
		self->y *= u.y;
		self->z *= u.z;
	}
	else if (self->type == T_VEC && u.type == T_DOUBLE)
	{
		self->x *= u.num;
		self->y *= u.num;
		self->z *= u.num;
	}
	else if (self->type == T_DOUBLE && u.type == T_DOUBLE)
		self->num *= u.num;
}

t_vec	vec_init(void)
{
	t_vec	res;

	ft_bzero(&res, sizeof(t_vec));
	res.cpy = &set_cpy;
	res.length_squared = &set_length_squared;
	res.length = &set_length;
	res.minus = &set_minus;
	res.subt = &set_subt;
	res.add = &set_add;
	res.mul = &set_mul;
	return (res);
}

t_vec	vec_set_double(double _num)
{
	t_vec	res;

	res = vec_init();
	res.type = T_DOUBLE;
	res.num = _num;
	return (res);
}

t_vec	vec_set_vec(double _x, double _y, double _z)
{
	t_vec	res;

	res = vec_init();
	res.type = T_VEC;
	res.x = _x;
	res.y = _y;
	res.z = _z;
	return (res);
}

t_vec	vec_add(const t_vec u, const t_vec v)
{
	if (u.type == T_VEC && v.type == T_VEC)
		return (vec_set_vec((u.x) + (v.x), \
						(u.y) + (v.y), \
						(u.z) + (v.z)));
	return (vec_init());
}

t_vec	vec_minus(const t_vec u, const t_vec v)
{
	if (u.type == T_VEC && v.type == T_VEC)
		return (vec_set_vec((u.x) - (v.x), \
						(u.y) - (v.y), \
						(u.z) - (v.z)));
	return (vec_init());
}

t_vec	vec_mul_double_vec(const double tmp, const t_vec vec)
{
	if (vec.type == T_VEC)
		return (vec_set_vec((tmp) * (vec.x), \
							(tmp) * (vec.y), \
							(tmp) * (vec.z)));
	return (vec_init());
}

t_vec	vec_mul(const t_vec u, const t_vec v)
{
	if (u.type == T_VEC && v.type == T_VEC)
		return (vec_set_vec((u.x) * (v.x), \
					(u.y) * (v.y), \
					(u.z) * (v.z)));
	if (u.type == T_DOUBLE && v.type == T_VEC)
		return (vec_mul_double_vec(u.num, v));
	if (u.type == T_VEC && v.type == T_DOUBLE)
		return (vec_mul_double_vec(v.num, u));
	return (vec_init());
}

t_vec	vec_div(const t_vec u, const double t)
{
	t_vec	d_num;

	if (u.type == T_NAN)
		return (vec_init());
	d_num.type = T_DOUBLE;
	d_num.num = 1 / (t);
	return (vec_mul(u, d_num));
}

double	vec_dot(const t_vec u, const t_vec v)
{
	if (u.type != T_VEC || v.type != T_VEC)
		return (0.0);
	return ((u.x) * (v.x) + \
			(u.y) * (v.y) + \
			(u.z) * (v.z));
}

t_vec	vec_cross(const t_vec u, const t_vec v)
{
	if (u.type != T_VEC || v.type != T_VEC)
		return (vec_init());
	return (vec_set_vec((u.y) * (v.z) - (u.z) * (v.y), \
						(u.z) * (v.x) - (u.x) * (v.z), \
						(u.x) * (v.y) - (u.y) * (v.x)));
}

t_vec	vec_unit(const t_vec u)
{
	return (vec_div(u, u.length(&u)));
}
