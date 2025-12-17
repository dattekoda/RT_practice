#include "world.h"
#include "color.h"
#include "vec3.h"
#include "rt_utils.h"

int	set_objects(t_hit_node **tree, t_list *line_lst);
static char	*get_word_line(t_list *line_lst, const char *word);

t_camera	set_camera(t_list *line_lst)
{
	char		*line;
	t_point3	origin;
	t_vec3		direct;
	double		vfov;

	line = get_word_line(line_lst, "C");
	origin = get_vec(&line);
	skip_spaces(&line);
	direct = get_vec(&line);
	skip_spaces(&line);
	vfov = ft_strtod(line, &line);
	return (construct_camera(origin, direct, vfov));
}

t_color	set_back_ground(t_list *line_lst)
{
	char	*line;
	double	lighting_ratio;
	t_color	back_ground;

	line = get_word_line(line_lst, "A");
	lighting_ratio = ft_strtod(line, &line);
	skip_spaces(&line);
	back_ground = get_vec(&line);
	back_ground = scal_mul_vec(back_ground, lighting_ratio);
	back_ground = construct_color(back_ground.x, back_ground.y, back_ground.z);
	return (back_ground);
}

int	set_world(t_world *world, t_list *line_lst)
{
	ft_bzero(world, sizeof(t_world));
	world->camera = set_camera(line_lst);
	world->back_ground = set_back_ground(line_lst);
	if (set_objects(&world->object_tree, line_lst))
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

/*
@brief 先頭文字がwordの文字列を最後スペーススキップしたうえで返す関数
*/
static char	*get_word_line(t_list *line_lst, const char *word)
{
	char	*line;
	size_t	word_len;

	word_len = ft_strlen(word);
	while (line_lst)
	{
		line = (char *)line_lst->content;
		if (ft_strncmp(line, word, word_len) == 0)
			break ;
		line_lst = line_lst->next;
	}
	line += word_len;
	skip_spaces(&line);
	return (line);
}
