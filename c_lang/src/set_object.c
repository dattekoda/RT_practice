#include "material.h"
#include "sphere.h"
#include "libft.h"
#include "rt_utils.h"
#include "lambertian.h"
#include "bvh_info.h"
#include "hit_node.h"
#include <stdlib.h>

#include "ray.h"
#include <stdio.h>
void	print_bvh_info(t_bvh_info *info, int size);


static bool		is_object(char *line);
static size_t	count_object_num(t_list *line_ptr);

// void checker_tree(t_hit_node *tree)
// {
// 	t_ray			tmp_ray;
// 	t_range			tmp_range;
// 	t_hit_record	rec;

// 	tmp_ray = construct_ray(construct_vec(0,0,0), \
// construct_vec(0,0,-1));
// 	tmp_range = construct_range(0.001, 100000);
// 	if (tree->lhs)
// 		checker_tree(tree->lhs);
// 	if (tree->data)
// 		tree->data->hit(tree->data, tmp_ray, &rec, tmp_range);
// 	if (tree->rhs)
// 		checker_tree(tree->rhs);
// }

int	set_objects(t_hit_node **tree, t_list *line_lst)
{
	t_bvh_info	*object_array;
	size_t		array_size;

	object_array = (t_bvh_info *)calloc(count_object_num(line_lst) + 1, sizeof(t_bvh_info));
	if (!object_array)
		return (perror("malloc"), EXIT_FAILURE);
	array_size = 0;
	while (line_lst)
	{
		if (is_object(line_lst->content))
			object_array[array_size++] = construct_bvh_info(line_lst->content);
		line_lst = line_lst->next;
	}
	*tree = construct_bvh(object_array, 0, array_size - 1);
	// print_bvh_info(object_array, 3);
	// checker_tree(*tree);
	// exit(1);
	return (EXIT_SUCCESS);
}

static bool	is_object(char *line)
{
	static const char	*object_list[2] = {
		"sp", NULL
	};
	size_t	i;

	i = 0;
	while (object_list[i])
	{
		if (ft_strncmp(line, object_list[i], 2) == 0)
			return (true);
		i++;
	}
	return (false);
}

static size_t	count_object_num(t_list *line_lst)
{
	size_t	count;

	count = 0;
	while (line_lst)
	{
		if (is_object(line_lst->content))
			count++;
		line_lst = line_lst->next;
	}
	return (count);
}
