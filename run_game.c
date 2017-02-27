/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_game.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhurt <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/26 22:19:17 by lhurt             #+#    #+#             */
/*   Updated: 2017/02/26 22:19:18 by lhurt            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf.h"

void	get_fps(t_env *obj)
{
	double	old_time;
	char	*print;
	char	*tmp;

	old_time = obj->var.cur_time;
	obj->var.cur_time = get_time();
	if (obj->var.cur_time < old_time)
		obj->var.cur_time += 1000000000;
	obj->var.frame_time = (double)(obj->var.cur_time - old_time) / 1000000000.0;
	print = ft_strnew(5);
	print = "FPS: ";
	tmp = ft_itoa((int)1.0 / obj->var.frame_time);
	print = ft_strjoin(print, tmp);
	mlx_string_put(obj->mlx.mlx, obj->mlx.win, 0, 0, 0xFFFFFF, print);
	ft_strdel(&tmp);
	ft_strdel(&print);
}

void	draw_chline(t_env *obj, int prim, int secn, int check)
{
	int x;
	int y;

	x = prim + CH_LEN + 1;
	while (x-- > prim - CH_LEN)
	{
		y = secn + CH_WID + 1;
		while (y-- > secn - CH_WID)
		{
			if (x > prim + CH_OFF || x < prim - CH_OFF)
			{
				if (check)
					mlx_pixel_put(obj->mlx.mlx, obj->mlx.win, x, y, CH_COLOR);
				else
					mlx_pixel_put(obj->mlx.mlx, obj->mlx.win, y, x, CH_COLOR);
			}
		}
	}
}

void	draw_crosshair(t_env *obj)
{
	draw_chline(obj, X_ORIGIN, Y_ORIGIN, 1);
	draw_chline(obj, Y_ORIGIN, X_ORIGIN, 0);
}

int		run_img(t_env *obj)
{
	run_keys(obj);
	run_mouse(obj);
	obj->var.cur_time = get_time();
	init_rays(obj);
	handle_sprites(obj);
	mlx_put_image_to_window(obj->mlx.mlx, obj->mlx.win, obj->mlx.img, 0, 0);
	get_fps(obj);
	draw_crosshair(obj);
	return (0);
}

void	run_game(t_env *obj)
{
	if (obj->mlx.win)
		mlx_clear_window(obj->mlx.mlx, obj->mlx.win);
	else
		obj->mlx.win = mlx_new_window(obj->mlx.mlx, W_WIDTH, W_HEIGHT,
			"Wolf3D");
	if (obj->mlx.img)
		mlx_destroy_image(obj->mlx.mlx, obj->mlx.img);
	obj->mlx.img = mlx_new_image(obj->mlx.mlx, W_WIDTH, W_HEIGHT);
	obj->mlx.data = mlx_get_data_addr(obj->mlx.img, &obj->mlx.bits,
		&obj->mlx.size_line, &obj->mlx.endian);
	mlx_do_key_autorepeatoff(obj->mlx.mlx);
	mlx_hook(obj->mlx.win, 6, 0, my_mouse_movement, obj);
	mlx_hook(obj->mlx.win, 2, 0, my_key_press, obj);
	mlx_hook(obj->mlx.win, 3, 0, my_key_release, obj);
	mlx_hook(obj->mlx.win, 17, 0, exit_hook, obj);
	mlx_loop_hook(obj->mlx.mlx, run_img, obj);
}
