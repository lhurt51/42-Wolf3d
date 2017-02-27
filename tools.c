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

int				count_ord(char *av)
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

int				exit_hook(t_env *obj)
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

int				charhextoint(char *str, int i)
{
	char	*hex_d;
	int		j;
	int		power;
	int		ans;

	power = 0;
	ans = 0;
	hex_d = "0123456789ABCDEF";
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
