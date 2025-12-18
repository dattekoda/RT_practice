#ifndef IMG_H
# define IMG_H

typedef struct s_hook_var
{
	void	*mlx;
	void	*win;
}	t_hook_var;

typedef struct s_img {
	void	*id;
	char	*addr;
	int		bytes_per_pixels;
	int		line_size;
	int		endian;
}	t_img;

void	construct_img(t_img *img, void *mlx);
void	initialize(t_hook_var *var, t_img *img);

#endif