/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loading.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhurt <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/26 22:10:28 by lhurt             #+#    #+#             */
/*   Updated: 2017/02/26 22:10:30 by lhurt            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf.h"

void	loaded(t_env *obj)
{
	int	*ptr;
	int	size;

	obj->mlx.img = mlx_xpm_file_to_image(obj->mlx.mlx,
		"XMP_textures/Wolfenstein-teaser2.XPM", &size, &size);
	mlx_put_image_to_window(obj->mlx.mlx, obj->mlx.win, obj->mlx.img, 0, 0);
	mlx_string_put(obj->mlx.mlx, obj->mlx.win, 5, 5, 0xFFFFFF,
		"Game Build by Liam Hurt, Only Intended for Educational Purposes");
	mlx_string_put(obj->mlx.mlx, obj->mlx.win, X_ORIGIN - 70, Y_ORIGIN,
		0xFFFFFF, "Wolfenstein 3D");
	mlx_string_put(obj->mlx.mlx, obj->mlx.win, X_ORIGIN - 165, Y_ORIGIN + 275,
		0xFFFFFF, "-- Press any key to continue! --");
	if (obj->load_per == 102)
	{
		if (!obj->m_env.map || !*obj->tex)
			exit_hook(obj);
		pthread_join(obj->pth, (void**)&(ptr));
		obj->load_per++;
	}
}

void	loading_files(t_env *obj)
{
	int x;
	int	y;

	x = 29;
	while (x++ < (int)(W_WIDTH * (obj->load_per * 0.01)) && x <= W_WIDTH - 30)
	{
		y = Y_ORIGIN - 21;
		while (y++ < Y_ORIGIN + 20)
			pixel_to_img(obj, x, y, 0xFFFFFF);
	}
	mlx_put_image_to_window(obj->mlx.mlx, obj->mlx.win, obj->mlx.img, 0, 0);
	if (obj->load_per > 100)
		mlx_string_put(obj->mlx.mlx, obj->mlx.win, X_ORIGIN - 90, Y_ORIGIN +40,
			0xFFFFFF, "Loading Completed!");
}

int		start_loading(t_env *obj)
{
	if (obj->load_per >= 102)
		loaded(obj);
	else
		loading_files(obj);
	return (0);
}
