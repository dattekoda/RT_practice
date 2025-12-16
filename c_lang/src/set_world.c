#include "world.h"
#include "color.h"

t_world	set_world(void)
{
	t_world	world;

	world.back_ground = construct_color(0.0f, 0.0f, 0.0f);
	world.camera = construct_camera( \
		construct_vec(0, 1, 0), \
		normalize(construct_vec(0, -0.3, -1)), \
		70);
}
