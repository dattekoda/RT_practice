#ifndef WORLD_H
# define WORLD_H

# include "camera.h"
# include "color.h"
# include "hit_node.h"

typedef struct s_world {
	t_camera	camera;
	t_color		back_ground;
	t_hit_node	objects;
}	t_world;

#endif