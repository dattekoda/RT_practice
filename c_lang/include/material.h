#ifndef MATERIAL_H
# define MATERIAL_H

#include "hit_record.h"
#include "color.h"
#include "ray.h"
#include <stdbool.h>

typedef struct s_material
{
	bool	(*scatter)(const void *self, t_hit_record rec, t_color *attenuation, t_ray *scattered);
	t_color	(*emitted)(const void *self, t_hit_record rec);
}	t_material;

t_color	emitted_non_light(const void *s, t_hit_record rec);

#endif