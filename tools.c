/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhurt <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/24 17:14:49 by lhurt             #+#    #+#             */
/*   Updated: 2017/01/24 17:14:51 by lhurt            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf.h"

void			*error(char *msg)
{
	ft_putstr("Error: ");
	ft_putendl(msg);
	return (NULL);
}

int			count_ord(char *av)
{
	int				fd;
	char			*tmp;
	int				count;

	count = 0;
	fd = open(av, O_RDONLY);
	if (fd < 0)
		return (0);
	while (get_next_line(fd, &tmp))
		count++;
	close(fd);
	return (count);
}

int		exit_hook(t_env *obj)
{
	if (obj->mlx.win)
		mlx_destroy_window(obj->mlx.mlx, obj->mlx.win);
	if (obj->mlx.img)
		mlx_destroy_image(obj->mlx.mlx, obj->mlx.img);
	if (obj->m_env.sprites)
		ft_memdel((void**)&obj->m_env.sprites);
	if (obj->m_env.map)
		ft_memdel((void**)obj->m_env.map);
	if (obj->m_env.av)
		ft_strdel(&obj->m_env.av);
	free(obj);
	exit(0);
}

unsigned int	count_lines(char **str)
{
	unsigned int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

void	swap_mem(double *dst, double *src)
{
	double	tmp;

	tmp = *src;
	*src = *dst;
	*dst = tmp;
}

void	swap_memin(int *dst, int *src)
{
	int	tmp;

	tmp = *src;
	*src = *dst;
	*dst = tmp;
}

int		charhextoint(char *str, int i)
{
	char hex_d[16] = "0123456789ABCDEF";
	int	j;
	int	power;
	int ans;

	power = 0;
	ans = 0;
	while (i >= 0)
	{
		j = 0;
		while (j < 16)
		{
			if (str[i] == hex_d[j])
				ans += j * pow(16, power);
			j++;
		}
		power++;
		i--;
	}
	return (ans);
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
