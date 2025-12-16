#include "libft.h"
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

#define BUFFER_SIZE 4096

int	set_line_lst(t_list **line_lst, const char *file_name)
{
	int		fd;
	int		gnl_ret;
	char	*line;
	t_list	head;
	t_list	*cur;

	fd = open(file_name, O_RDONLY);
	ft_bzero(&head, sizeof(t_list));
	cur = &head;
	if (fd == -1)
		return (perror("open"), EXIT_FAILURE);
	while (cur)
	{
		gnl_ret = ft_get_next_line(fd, &line);
		if (gnl_ret == -1)
			return (perror("read"), EXIT_FAILURE);
		if (gnl_ret == -2)
			return (perror("malloc"), EXIT_FAILURE);
		if (gnl_ret == 0)
			break ;
		cur->next = ft_lstnew(line);
		cur = cur->next;
	}
	*line_lst = head.next;
	return (EXIT_SUCCESS);
}

// int	main(int argc, char *argv[])
// {
// 	t_list	*head;
// 	t_list	*lst;

// 	if (argc == 1)
// 		return 1;
// 	if (set_line_lst(&lst, argv[1]))
// 		return (1);
// 	head = lst;
// 	while (lst) {
// 		printf("%s", (char *)(lst->content));
// 		lst = lst->next;
// 	}
// 	ft_lstclear(&head, free);
// 	return (0);
// }
