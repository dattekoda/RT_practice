#ifndef VEC3_H
# define VEC3_H

# include <math.h>

typedef enum s_type
{
	T_NAN,
	T_VEC,
	T_DOUBLE
}	t_type;

typedef struct s_vec t_vec;

typedef	struct s_vec
{
	t_type	type;
	double	num;
	double	x;
	double	y;
	double	z;
	void	(*cpy)(t_vec*, const t_vec);
	double	(*length_squared)(const t_vec*);
	double	(*length)(const t_vec*);
	t_vec	(*minus)(const t_vec*);
	void	(*add)(t_vec*, const t_vec);
	void	(*subt)(t_vec*, const t_vec);
	void	(*mul)(t_vec*, const t_vec);
}	t_vec;

t_vec	vec_init(void);
t_vec	vec_set_double(double _num);
t_vec	vec_set_vec(double _x, double _y, double _z);
t_vec	vec_add(const t_vec u, const t_vec v);
t_vec	vec_minus(const t_vec u, const t_vec v);
t_vec	vec_mul_double_vec(const double tmp, const t_vec vec);
t_vec	vec_mul(const t_vec u, const t_vec v);
t_vec	vec_div(const t_vec u, const double t);
double	vec_dot(const t_vec u, const t_vec v);
t_vec	vec_cross(const t_vec u, const t_vec v);
t_vec	vec_unit(const t_vec u);

#endif