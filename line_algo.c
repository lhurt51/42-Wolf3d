/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_algo.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhurt <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/21 19:21:13 by lhurt             #+#    #+#             */
/*   Updated: 2016/11/21 19:23:15 by lhurt            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf.h"

void	draw_y(t_mlx *lst, t_intpoint *point1, t_intpoint *point2, t_line *new)
{
	int		x;
	int		y;
	int		p;

	x = point1->x;
	y = point1->y;
	p = 2 * new->xans - new->yans;
	if (x < point2->x)
		new->inc = 1;
	else
		new->inc = -1;
	while (y < point2->y)
	{
		if (x < W_WIDTH && y < W_HEIGHT)
			pixel_to_img(lst, x, y, lst->env.color);
		if (p < 0)
			p += 2 * new->xans;
		else
		{
			x += new->inc;
			p += 2 * (new->xans - new->yans);
		}
		y++;
	}
}

void	draw_x(t_mlx *lst, t_intpoint *point1, t_intpoint *point2, t_line *new)
{
	int		x;
	int		y;
	int		p;

	x = point1->x;
	y = point1->y;
	p = 2 * new->yans - new->xans;
	if (y < point2->y)
		new->inc = 1;
	else
		new->inc = -1;
	while (x < point2->x)
	{
		if (x < W_WIDTH && y < W_HEIGHT)
			pixel_to_img(lst, x, y, lst->env.color);
		if (p < 0)
			p += 2 * new->yans;
		else
		{
			y += new->inc;
			p += 2 * (new->yans - new->xans);
		}
		x++;
	}
}

void	get_da(t_mlx *lst, t_intpoint *point1, t_intpoint *point2)
{
	t_line		*new;

	new = malloc(sizeof(t_line));
	if (!new)
		return ;
	new->xans = abs(point2->x - point1->x);
	new->yans = abs(point2->y - point1->y);
	if (new->xans > new->yans)
	{
		if (point1->x > point2->x)
			draw_x(lst, point2, point1, new);
		else
			draw_x(lst, point1, point2, new);
	}
	else
	{
		if (point1->y > point2->y)
			draw_y(lst, point2, point1, new);
		else
			draw_y(lst, point1, point2, new);
	}
}
