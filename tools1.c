/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhurt <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/26 22:39:46 by lhurt             #+#    #+#             */
/*   Updated: 2017/02/26 22:39:48 by lhurt            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf.h"

void	*error(char *msg)
{
	ft_putstr("Error: ");
	ft_putendl(msg);
	return (NULL);
}

void	reset_struct(t_env *obj)
{
	obj->vec.pos.x = 11.0;
	obj->vec.pos.y = 11.0;
	obj->vec.dir.x = -1.0;
	obj->vec.dir.y = 0.0;
	obj->vec.plane.x = 0.0;
	obj->vec.plane.y = 0.66;
	obj->var.wall_x = 0;
	obj->load_per = 0;
	obj->mlx.keys.m_left = 0;
	obj->mlx.keys.m_right = 0;
	obj->mlx.keys.shift = 0;
	obj->mlx.keys.a = 0;
	obj->mlx.keys.s = 0;
	obj->mlx.keys.d = 0;
	obj->mlx.keys.w = 0;
}

double	get_time(void)
{
	clock_serv_t	cclock;
	mach_timespec_t	mts;

	host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
	clock_get_time(cclock, &mts);
	mach_port_deallocate(mach_task_self(), cclock);
	return (mts.tv_nsec);
}

void	pixel_to_img(t_env *new, int x, int y, int color)
{
	int		i;

	i = (x * 4) + (y * new->mlx.size_line);
	if (i > 0 && color >= 0)
	{
		new->mlx.data[i++] = color & 0xFF;
		new->mlx.data[i++] = (color >> 8) & 0xFF;
		new->mlx.data[i] = (color >> 16) & 0xFF;
	}
}
