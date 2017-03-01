/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_casting.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhurt <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/26 22:32:34 by lhurt             #+#    #+#             */
/*   Updated: 2017/02/26 22:32:36 by lhurt            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf.h"

void	set_env(t_env *obj, t_point *map, int x)
{
	obj->vec.ray_dir.x = obj->vec.dir.x + obj->vec.plane.x * CAMERA_X(x);
	obj->vec.ray_dir.y = obj->vec.dir.y + obj->vec.plane.y * CAMERA_X(x);
	map->x = (int)obj->vec.pos.x;
	map->y = (int)obj->vec.pos.y;
	obj->vec.delta_dist.x = DD(obj->vec.ray_dir.x, obj->vec.ray_dir.y);
	obj->vec.delta_dist.y = DD(obj->vec.ray_dir.y, obj->vec.ray_dir.x);
}

void	find_dir_step(t_env *obj, t_point *map, t_point *side_dist)
{
	if (obj->vec.ray_dir.x < 0)
	{
		obj->vec.step.x = -1;
		side_dist->x = (obj->vec.pos.x - map->x) * obj->vec.delta_dist.x;
	}
	else
	{
		obj->vec.step.x = 1;
		side_dist->x = (map->x + 1.0 - obj->vec.pos.x) * obj->vec.delta_dist.x;
	}
	if (obj->vec.ray_dir.y < 0)
	{
		obj->vec.step.y = -1;
		side_dist->y = (obj->vec.pos.y - map->y) * obj->vec.delta_dist.y;
	}
	else
	{
		obj->vec.step.y = 1;
		side_dist->y = (map->y + 1.0 - obj->vec.pos.y) * obj->vec.delta_dist.y;
	}
}

void	exe_dda(t_env *obj, t_point *map, t_point *side_dist)
{
	int hit;

	hit = 0;
	while (hit == 0)
	{
		if (side_dist->x < side_dist->y)
		{
			side_dist->x += obj->vec.delta_dist.x;
			map->x += obj->vec.step.x;
			obj->var.side = 0;
		}
		else
		{
			side_dist->y += obj->vec.delta_dist.y;
			map->y += obj->vec.step.y;
			obj->var.side = 1;
		}
		if (obj->m_env.map[(int)map->x][(int)map->y] > 0)
			hit = 1;
	}
}

void	init_rays(t_env *obj)
{
	int			x;
	t_point		side_dist;
	t_point		map;

	x = W_WIDTH + 1;
	while (x-- > 0)
	{
		set_env(obj, &map, x);
		find_dir_step(obj, &map, &side_dist);
		exe_dda(obj, &map, &side_dist);
		find_draw_pnts(obj, &map);
		choose_tex(obj, &map, x);
		obj->z_buff[x] = obj->var.wall_dist;
		floor_casting(obj, &map, x);
	}
}
