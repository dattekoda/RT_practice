#ifndef HIT_TABLE_H
# define HIT_TABLE_H

# include "hit_record.h"
# include "ray.h"
# include "material.h"
# include <stdbool.h>

typedef struct s_hit_table
{
	bool		(*hit)(const void *, const t_ray, t_hit_record *);
	t_material	*mat_ptr;
}	t_hit_table;

#endif