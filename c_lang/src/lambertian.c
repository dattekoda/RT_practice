#include "material.h"
#include "lambertian.h"
#include "libft.h"

bool	scatter_lambertian(const void *s, t_hit_record rec, t_color *attenuation, t_ray *scattered)
{
	t_lambertian	*self;
	t_vec3			scatter_direction;

	self = (t_lambertian *)s;
	scatter_direction = add_vec(rec.normal, random_unit_vector());
	if (dot(rec.normal, rec.ray_in.direct) > 0)
		scatter_direction = negative_vec(scatter_direction);
	*scattered = construct_ray(rec.point, scatter_direction);
	*attenuation = self->albedo;
	return (true);
}

t_lambertian	construct_lambertian(t_color _albedo)
{
	t_lambertian	lam;

	lam.material.scatter = scatter_lambertian;
	lam.material.emitted = emitted_non_light;
	lam.albedo = _albedo;
	return (lam);
}

t_lambertian	*gen_lambertian(t_color _albedo)
{
	t_lambertian	*p;

	p = ft_calloc(1, sizeof(t_lambertian));
	if (!p)
		return (NULL);
	*p = construct_lambertian(_albedo);
	return (p);
}
