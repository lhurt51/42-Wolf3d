/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_floor.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhurt <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/26 22:35:49 by lhurt             #+#    #+#             */
/*   Updated: 2017/02/26 22:35:51 by lhurt            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf.h"

void	draw_floor(t_env *obj, t_point *floor_wall, int x)
{
	t_point		cur_floor;
	t_point		floor_tex;
	double		cur_dist;
	double		weight;
	int			y;

	obj->var.draw_end = (obj->var.draw_end < 0) ? W_HEIGHT : obj->var.draw_end;
	y = obj->var.draw_end + 1;
	while (y <= W_HEIGHT)
	{
		cur_dist = W_HEIGHT / (2.0 * y - W_HEIGHT);
		weight = (cur_dist - 0.0) / (obj->var.wall_dist - 0.0);
		cur_floor.x = weight * floor_wall->x + (1.0 - weight) * obj->vec.pos.x;
		cur_floor.y = weight * floor_wall->y + (1.0 - weight) * obj->vec.pos.y;
		floor_tex.x = abs((int)(cur_floor.x * T_SIZE) % T_SIZE);
		floor_tex.y = abs((int)(cur_floor.y * T_SIZE) % T_SIZE);
		if ((x < W_WIDTH && x >= 0) && (y < W_HEIGHT && y >= 0))
			pixel_to_img(obj, x, y,
				obj->tex[0][(int)floor_tex.y][(int)floor_tex.x]);
		if ((x < W_WIDTH && x >= 0) && (W_HEIGHT - y < W_HEIGHT &&
				W_HEIGHT - y >= 0))
			pixel_to_img(obj, x, W_HEIGHT - y,
				obj->tex[17][(int)floor_tex.y][(int)floor_tex.x]);
		y++;
	}
}

void	floor_casting(t_env *obj, t_point *map, int x)
{
	t_point		floor_wall;

	if (obj->var.side == 0 && obj->vec.ray_dir.x > 0)
	{
		floor_wall.x = map->x;
		floor_wall.y = map->y + obj->var.wall_x;
	}
	else if (obj->var.side == 0 && obj->vec.ray_dir.x < 0)
	{
		floor_wall.x = map->x + 1.0;
		floor_wall.y = map->y + obj->var.wall_x;
	}
	else if (obj->var.side == 1 && obj->vec.ray_dir.y > 0)
	{
		floor_wall.x = map->x + obj->var.wall_x;
		floor_wall.y = map->y;
	}
	else
	{
		floor_wall.x = map->x + obj->var.wall_x;
		floor_wall.y = map->y + 1.0;
	}
	draw_floor(obj, &floor_wall, x);
}
