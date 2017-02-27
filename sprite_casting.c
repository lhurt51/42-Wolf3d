/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprites.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhurt <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/26 22:26:46 by lhurt             #+#    #+#             */
/*   Updated: 2017/02/26 22:26:47 by lhurt            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf.h"

t_point	find_sprite(t_env *obj, int tmp)
{
	t_point	sprite;
	t_point	trans;
	double	mat;

	sprite.x = obj->m_env.sprites[tmp].pnt.x - obj->vec.pos.x;
	sprite.y = obj->m_env.sprites[tmp].pnt.y - obj->vec.pos.y;
	mat = 1.0 / (obj->vec.plane.x * obj->vec.dir.y - obj->vec.dir.x *
		obj->vec.plane.y);
	trans.x = mat * (obj->vec.dir.y * sprite.x - obj->vec.dir.x * sprite.y);
	trans.y = mat * (-obj->vec.plane.y * sprite.x + obj->vec.plane.x * sprite.y);
	obj->m_env.sprite_dm.x = (int)((X_ORIGIN * (1 + trans.x / trans.y)));
	obj->m_env.sprite_dm.y = abs((int)(W_HEIGHT / trans.y));
	obj->m_env.start.x = -obj->m_env.sprite_dm.y / 2 + obj->m_env.sprite_dm.x;
	if (obj->m_env.start.x < 0)
		obj->m_env.start.x = 0;
	obj->m_env.start.y = -obj->m_env.sprite_dm.y / 2 + Y_ORIGIN;
	if (obj->m_env.start.y < 0)
		obj->m_env.start.y = 0;
	obj->m_env.end.x = obj->m_env.sprite_dm.y / 2 + obj->m_env.sprite_dm.x;
	if (obj->m_env.end.x >= W_WIDTH)
		obj->m_env.end.x = W_WIDTH - 1;
	obj->m_env.end.y = obj->m_env.sprite_dm.y / 2 + Y_ORIGIN;
	if (obj->m_env.end.y >= W_HEIGHT)
		obj->m_env.end.y = W_HEIGHT - 1;
	return (trans);
}

void	draw_sprites(t_env *obj, t_point trans, int stripe, int tmp)
{
	t_point tex;
	int		color;
	int		ans;
	int		y;

	while (++stripe < obj->m_env.end.x)
	{
		tex.x = abs((int)(256*(stripe-(-obj->m_env.sprite_dm.y/
			2+obj->m_env.sprite_dm.x))*T_SIZE/obj->m_env.sprite_dm.y)/256);
		if (trans.y>0&&stripe>0&&stripe<W_WIDTH &&trans.y<obj->z_buff[stripe])
		{
			y = obj->m_env.start.y - 1;
			while (++y < obj->m_env.end.y)
			{
				ans = y * 256 - W_HEIGHT * 128 + obj->m_env.sprite_dm.y * 128;
				tex.y = abs((int)((ans * T_SIZE)/obj->m_env.sprite_dm.y)/256);
				color = obj->tex[tmp][(int)tex.y][(int)tex.x];
				if ((color != 0x980088 && color != 0x9B038B && color >= 0) &&
				(stripe < W_WIDTH && stripe >= 0) &&
				(y < W_HEIGHT && y >= 0))
					pixel_to_img(obj, stripe, y, color);
			}
		}
	}
}

void	create_sprites(t_env *obj, t_sort *ord)
{
	t_point trans;
	int		tmp;
	int		i;

	i = obj->m_env.num_s;
	while (i-- > 0)
	{
		tmp = ord[i].index;
		trans = find_sprite(obj, tmp);
		draw_sprites(obj, trans, obj->m_env.start.x - 1,
			obj->m_env.sprites[tmp].tex);
	}
}

void	handle_sprites(t_env *obj)
{
	t_sort	*sort;
	int		i;

	i = obj->m_env.num_s;
	sort = malloc(sizeof(t_sort) * obj->m_env.num_s);
	if (!sort)
		exit_hook(obj);
	while (i-- > 0)
	{
		sort[i].index = i;
		sort[i].dist = ((obj->vec.pos.x - obj->m_env.sprites[i].pnt.x) *
		(obj->vec.pos.x  - obj->m_env.sprites[i].pnt.x) +
		(obj->vec.pos.y - obj->m_env.sprites[i].pnt.y) *
		(obj->vec.pos.y - obj->m_env.sprites[i].pnt.y));
	}
	ft_qsort(sort, obj->m_env.num_s - 1, 0);
	create_sprites(obj, sort);
	ft_memdel((void**)&sort);
}
