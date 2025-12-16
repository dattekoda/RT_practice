#ifndef OBJECT_LIST_H
# define OBJECTS_LIST_H

#include "hit_table.h"

typedef struct s_objects_list t_objects_list;

struct s_objects_list
{
	t_hit_table		*data;
	t_objects_list	*next;
};

#endif