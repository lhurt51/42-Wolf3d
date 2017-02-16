/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_hooks.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhurt <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/15 14:39:41 by lhurt             #+#    #+#             */
/*   Updated: 2017/02/15 14:39:44 by lhurt            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf.h"

int		worldmap1[M_WIDTH][M_HEIGHT] =
{
	{8,8,8,8,8,8,8,8,8,8,8,4,4,6,4,4,6,4,6,4,4,4,6,4},
	{8,0,0,0,0,0,0,0,0,0,8,4,0,0,0,0,0,0,0,0,0,0,0,4},
	{8,0,3,3,0,0,0,0,0,8,8,4,0,0,0,0,0,0,0,0,0,0,0,6},
	{8,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6},
	{8,0,3,3,0,0,0,0,0,8,8,4,0,0,0,0,0,0,0,0,0,0,0,4},
	{8,0,0,0,0,0,0,0,0,0,8,4,0,0,0,0,0,6,6,6,0,6,4,6},
	{8,8,8,8,0,8,8,8,8,8,8,4,4,4,4,4,4,6,0,0,0,0,0,6},
	{7,7,7,7,0,7,7,7,7,0,8,0,8,0,8,0,8,4,0,4,0,6,0,6},
	{7,7,0,0,0,0,0,0,7,8,0,8,0,8,0,8,8,6,0,0,0,0,0,6},
	{7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8,6,0,0,0,0,0,4},
	{7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8,6,0,6,0,6,0,6},
	{7,7,0,0,0,0,0,0,7,8,0,8,0,8,0,8,8,6,4,6,0,6,6,6},
	{7,7,7,7,0,7,7,7,7,8,8,4,0,6,8,4,8,3,3,3,0,3,3,3},
	{2,2,2,2,0,2,2,2,2,4,6,4,0,0,6,0,6,3,0,0,0,0,0,3},
	{2,2,0,0,0,0,0,2,2,4,0,0,0,0,0,0,4,3,0,0,0,0,0,3},
	{2,0,0,0,0,0,0,0,2,4,0,0,0,0,0,0,4,3,0,0,0,0,0,3},
	{1,0,0,0,0,0,0,0,1,4,4,4,4,4,6,0,6,3,3,0,0,0,3,3},
	{2,0,0,0,0,0,0,0,2,2,2,1,2,2,2,6,6,0,0,5,0,5,0,5},
	{2,2,0,0,0,0,0,2,2,2,0,0,0,2,2,0,5,0,5,0,0,0,5,5},
	{2,0,0,0,0,0,0,0,2,0,0,0,0,0,2,5,0,5,0,5,0,5,0,5},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5},
	{2,0,0,0,0,0,0,0,2,0,0,0,0,0,2,5,0,5,0,5,0,5,0,5},
	{2,2,0,0,0,0,0,2,2,2,0,0,0,2,2,0,5,0,5,0,0,0,5,5},
	{2,2,2,2,1,2,2,2,2,2,2,1,2,2,2,5,5,5,5,5,5,5,5,5}
};

void	move_up(t_env *obj)
{
	double	move_speed;

	move_speed = obj->var.frame_time * (2.25 + obj->var.sprint);
	if(worldmap1[(int)(obj->vec.pos.x + obj->vec.dir.x * move_speed)][(int)obj->vec.pos.y] == 0)
		obj->vec.pos.x += obj->vec.dir.x * move_speed;
	if(worldmap1[(int)obj->vec.pos.x][(int)(obj->vec.pos.y + obj->vec.dir.y * move_speed)] == 0)
		obj->vec.pos.y += obj->vec.dir.y * move_speed;
}

void	move_down(t_env *obj)
{
	double	move_speed;

	move_speed = obj->var.frame_time * (2.25 + obj->var.sprint);
	if(worldmap1[(int)(obj->vec.pos.x - obj->vec.dir.x * move_speed)][(int)obj->vec.pos.y] == 0)
		obj->vec.pos.x -= obj->vec.dir.x * move_speed;
	if(worldmap1[(int)obj->vec.pos.x][(int)(obj->vec.pos.y - obj->vec.dir.y * move_speed)] == 0)
		obj->vec.pos.y -= obj->vec.dir.y * move_speed;
}

void	move_right(t_env *obj)
{
	double	move_speed;

	move_speed = obj->var.frame_time * (2.0 + obj->var.sprint);
	if(worldmap1[(int)(obj->vec.pos.x + obj->vec.plane.x * move_speed)][(int)obj->vec.pos.y] == 0)
		obj->vec.pos.x += obj->vec.plane.x * move_speed;
	if(worldmap1[(int)obj->vec.pos.x][(int)(obj->vec.pos.y + obj->vec.plane.y * move_speed)] == 0)
		obj->vec.pos.y += obj->vec.plane.y * move_speed;
}

void	move_left(t_env *obj)
{
	double	move_speed;

	move_speed = obj->var.frame_time * (2.0 + obj->var.sprint);
	if(worldmap1[(int)(obj->vec.pos.x - obj->vec.plane.x * move_speed)][(int)obj->vec.pos.y] == 0)
		obj->vec.pos.x -= obj->vec.plane.x * move_speed;
	if(worldmap1[(int)obj->vec.pos.x][(int)(obj->vec.pos.y - obj->vec.plane.y * move_speed)] == 0)
		obj->vec.pos.y -= obj->vec.plane.y * move_speed;
}

void	run_keys(t_env *obj)
{
	obj->var.sprint = 0.0;
	if (obj->mlx.keys.shift == 1)
		obj->var.sprint = MOVE_BUFF;
	if (obj->mlx.keys.a == 1)
		move_left(obj);
	if (obj->mlx.keys.s == 1)
		move_down(obj);
	if (obj->mlx.keys.d == 1)
		move_right(obj);
	if (obj->mlx.keys.w == 1)
		move_up(obj);
}

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
	if(keycode == 2)
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
	if(keycode == 2)
		obj->mlx.keys.d = 0;
	if (keycode == 13)
		obj->mlx.keys.w = 0;
	return (0);
}
