#include "world.h"
#include "sphere.h"
#include <stdio.h>
void	print_sphere(const t_sphere *s)
{
	fprintf(stderr, "\nsphere\n");
	check_vec(s->center);
	fprintf(stderr, "r: %f\n", s->radius);
}

void	print_obj(const t_hit_node *node)
{
	if (!node)
		return ;
	if (node->data)
	{
		print_sphere((const t_sphere *)node->data);
		return ;
	}
	print_obj(node->lhs);
	print_obj(node->rhs);
}

void	print_world(const t_world *world)
{
	const t_camera *camera = &world->camera;
	fprintf(stderr, "world\n");
	fprintf(stderr, "camera:\n");
	check_vec(camera->higher_left_corner);
	check_vec(camera->horizontal);
	check_vec(camera->vertical);
	check_vec(camera->origin);
	print_obj(world->object_tree);
	exit(1);
}

void	print_bvh_info(t_bvh_info *info, int size)
{
	for (int i = 0; i < size ;i++)
		check_vec(info[i].centroid);
	fprintf(stderr, "\n");
}