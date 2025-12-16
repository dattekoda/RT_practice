#include "objects_list.h"
#include <stdlib.h>

t_objects_list	*add_object(t_hit_table *_data)
{
	t_objects_list	*node;

	node = calloc(1, sizeof(t_objects_list));
	if (!node)
		return (NULL);
	node->data = _data;
	return (node);
}
