/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys_and_mouse.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhurt <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/26 23:03:47 by lhurt             #+#    #+#             */
/*   Updated: 2017/02/26 23:03:49 by lhurt            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf.h"

int		my_key_press(int keycode, t_env *obj)
{
	if (keycode == 53)
		exit_hook(obj);
	if (keycode == 257)
		obj->mlx.keys.shift = 1;
	if (keycode == 0)
		obj->mlx.keys.a = 1;
	if (keycode == 1)
		obj->mlx.keys.s = 1;
	if (keycode == 2)
		obj->mlx.keys.d = 1;
	if (keycode == 13)
		obj->mlx.keys.w = 1;
	return (0);
}

int		my_key_release(int keycode, t_env *obj)
{
	if (keycode == 257)
		obj->mlx.keys.shift = 0;
	if (keycode == 0)
		obj->mlx.keys.a = 0;
	if (keycode == 1)
		obj->mlx.keys.s = 0;
	if (keycode == 2)
		obj->mlx.keys.d = 0;
	if (keycode == 13)
		obj->mlx.keys.w = 0;
	return (0);
}

int		my_mouse_movement(int x, int y, t_env *obj)
{
	obj->mlx.keys.m_left = 0;
	obj->mlx.keys.m_right = 0;
	if (x > X_ORIGIN + ROT_BUFF && (x <= W_WIDTH && y >= 0 && y <= W_HEIGHT))
	{
		obj->mlx.m_off = fabs((double)((x - (X_ORIGIN + ROT_BUFF)) /
			100) / 3.5);
		obj->mlx.keys.m_right = 1;
		obj->mlx.keys.m_left = 0;
	}
	else if (x < X_ORIGIN - ROT_BUFF && (x >= 0 && y >= 0 && y <= W_HEIGHT))
	{
		obj->mlx.m_off = fabs((double)((x - (X_ORIGIN - ROT_BUFF)) /
			100) / 3.5);
		obj->mlx.keys.m_left = 1;
		obj->mlx.keys.m_right = 0;
	}
	return (0);
}
