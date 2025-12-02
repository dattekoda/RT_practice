#include "vec3.h"
#include "mlx.h"
#include "mlx_int.h"
#include <stdio.h>

int	main(void)
{
	t_vec		u;
	t_xvar		*mlx_ptr;
	t_win_list	*win_list;
	t_img		*img;

	u = vec_set_vec(1, 2, 3);
	u.add(&u, vec_set_vec(2, 1, 3));
	printf("%f\n", u.x);
	mlx_ptr = mlx_init();
	if (!mlx_ptr)
		return 1;
	win_list = mlx_new_window(&mlx_ptr, 1920, 1080, "miniRT");
	if (!win_list)
		return (free(mlx_ptr), 1);
	img = mlx_new_image(mlx_ptr, 1920, 1080);
	if (!img)
		return (free(mlx_ptr), free(win_list), 1);
	img->data = mlx_get_data_addr(img, &img->bpp, &img->size_line, &img->image->byte_order);
	free(mlx_ptr);
	return 0;
}
