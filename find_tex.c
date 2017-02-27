/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_tex.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhurt <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/26 22:34:28 by lhurt             #+#    #+#             */
/*   Updated: 2017/02/26 22:34:29 by lhurt            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf.h"

void	find_draw_pnts(t_env *obj, t_point *map)
{
	if (obj->var.side == 0)
		obj->var.wall_dist = (map->x - obj->vec.pos.x + (1 - obj->vec.step.x) /
		2) / obj->vec.ray_dir.x;
	else
		obj->var.wall_dist = (map->y - obj->vec.pos.y + (1 - obj->vec.step.y) /
		2) / obj->vec.ray_dir.y;
	obj->var.line_h = (int)(W_HEIGHT / obj->var.wall_dist);
	obj->var.draw_start = -obj->var.line_h / 2 + Y_ORIGIN;
	if (obj->var.draw_start < 0)
		obj->var.draw_start = 0;
	obj->var.draw_end = obj->var.line_h / 2 + Y_ORIGIN;
	if (obj->var.draw_end >= W_HEIGHT)
		obj->var.draw_end = W_HEIGHT - 1;
}

int		set_tex_var(t_env *obj)
{
	int		tex_x;

	if (obj->var.side == 0)
		obj->var.wall_x = obj->vec.pos.y + obj->var.wall_dist *
		obj->vec.ray_dir.y;
	else
		obj->var.wall_x = obj->vec.pos.x + obj->var.wall_dist *
		obj->vec.ray_dir.x;
	obj->var.wall_x -= floor(obj->var.wall_x);
	tex_x = (int)(obj->var.wall_x * (double)(T_SIZE));
	if (obj->var.side == 0 && obj->vec.ray_dir.x > 0)
		tex_x = T_SIZE - tex_x - 1;
	if (obj->var.side == 1 && obj->vec.ray_dir.y < 0)
		tex_x = T_SIZE - tex_x - 1;
	return (tex_x);
}

void	choose_tex(t_env *obj, t_point *map, int x)
{
	t_point tex;
	int		ran;
	int		y;
	int		tex_num;

	tex_num = obj->m_env.map[(int)map->x][(int)map->y] - 1;
	tex_num = obj->tex_pal[tex_num][obj->var.side];
	tex.x = abs(set_tex_var(obj));
	y = (int)obj->var.draw_start;
	while (y < (int)obj->var.draw_end)
	{
		ran = y * 256 - W_HEIGHT * 128 + obj->var.line_h * 128;
		tex.y = abs(((ran * T_SIZE) / obj->var.line_h) / 256);
		if ((x < W_WIDTH && x >= 0) && (y < W_HEIGHT && y >= 0))
			pixel_to_img(obj, x, y, obj->tex[tex_num][(int)tex.y][(int)tex.x]);
		y++;
	}
}
