#ifndef WORLD_H
# define WORLD_H

# include "camera.h"
# include "color.h"
# include "hit_node.h"
# include "libft.h"

typedef struct s_world {
	t_hit_table	hit_table;
	t_camera	camera;
	t_color		back_ground;
	t_hit_node	*object_tree;
}	t_world;

#endif