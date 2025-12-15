#ifndef IMG_H
# define IMG_H

typedef struct s_img {
	void	*id;
	char	*addr;
	int		bytes_per_pixels;
	int		line_size;
	int		endian;
}	t_img;

void	construct_img(t_img *img, void *mlx);

#endif