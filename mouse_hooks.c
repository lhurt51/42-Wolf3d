/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_hooks.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhurt <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/15 14:39:53 by lhurt             #+#    #+#             */
/*   Updated: 2017/02/15 14:39:56 by lhurt            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf.h"

void	rot_right(t_env *obj, double off)
{
	double	old_dirx;
	double	old_planex;
	double	rot_speed;

	rot_speed = obj->var.frame_time * (M_PI * (0.125 + off));
	old_dirx = obj->vec.dir.x;
	old_planex = obj->vec.plane.x;
	obj->vec.dir.x = obj->vec.dir.x * cos(-rot_speed) - obj->vec.dir.y * sin(-rot_speed);
	obj->vec.dir.y = old_dirx * sin(-rot_speed) + obj->vec.dir.y * cos(-rot_speed);
	obj->vec.plane.x = obj->vec.plane.x * cos(-rot_speed) - obj->vec.plane.y * sin(-rot_speed);
	obj->vec.plane.y = old_planex * sin(-rot_speed) + obj->vec.plane.y * cos(-rot_speed);
}

void	rot_left(t_env *obj, double off)
{
	double	old_dirx;
	double	old_planex;
	double	rot_speed;

	rot_speed = obj->var.frame_time * (M_PI * (0.125 + off));
	old_dirx = obj->vec.dir.x;
	old_planex = obj->vec.plane.x;
	obj->vec.dir.x = obj->vec.dir.x * cos(rot_speed) - obj->vec.dir.y * sin(rot_speed);
	obj->vec.dir.y = old_dirx * sin(rot_speed) + obj->vec.dir.y * cos(rot_speed);
	obj->vec.plane.x = obj->vec.plane.x * cos(rot_speed) - obj->vec.plane.y * sin(rot_speed);
	obj->vec.plane.y = old_planex * sin(rot_speed) + obj->vec.plane.y * cos(rot_speed);
}

void 	run_mouse(t_env *obj)
{
	if (obj->mlx.keys.m_left == 1)
		rot_left(obj, obj->mlx.m_off);
	if (obj->mlx.keys.m_right == 1)
		rot_right(obj, obj->mlx.m_off);
}

int		my_mouse_movement(int x, int y, t_env *obj)
{
	obj->mlx.keys.m_left = 0;
	obj->mlx.keys.m_right = 0;
	if (x > X_ORIGIN + ROT_BUFF && (x <= W_WIDTH && y >= 0 && y <= W_HEIGHT))
	{
		obj->mlx.m_off = fabs((double)((x - (X_ORIGIN + ROT_BUFF)) / 100) / 3.5);
		obj->mlx.keys.m_right = 1;
		obj->mlx.keys.m_left = 0;
	}
	else if (x < X_ORIGIN - ROT_BUFF && (x >= 0 && y >= 0 && y <= W_HEIGHT))
	{
		obj->mlx.m_off = fabs((double)((x - (X_ORIGIN - ROT_BUFF)) / 100) / 3.5);
		obj->mlx.keys.m_left = 1;
		obj->mlx.keys.m_right = 0;
	}
	return (0);
}
