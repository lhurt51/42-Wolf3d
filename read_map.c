/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhurt <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/26 22:05:11 by lhurt             #+#    #+#             */
/*   Updated: 2017/02/26 22:05:12 by lhurt            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf.h"

int		store_info(t_env *obj, char *str)
{
	char	**tmp;
	int		i;

	tmp = ft_strsplit(str, ' ');
	if (count_lines(tmp) != 3)
		return (0);
	obj->m_env.num_s = ft_atoi(tmp[0]);
	obj->m_env.width = ft_atoi(tmp[1]);
	obj->m_env.height = ft_atoi(tmp[2]);
	obj->m_env.sprites = (t_sprite*)malloc(sizeof(t_sprite) * obj->m_env.num_s);
	if (!obj->m_env.sprites)
		return (0);
	obj->m_env.map = (int**)malloc(sizeof(int*) * obj->m_env.height);
	if (!obj->m_env.map)
		return (0);
	i = 0;
	while (tmp[i])
		ft_strdel(&tmp[i++]);
	return (1);
}

int		store_sprite(t_env *obj, char *str, int i)
{
	char **tmp;
	char *ext;

	tmp = ft_strsplit(str, '"');
	if (count_lines(tmp) != 2)
		return (0);
	obj->m_env.sprites[i].tex = ft_atoi(tmp[1]);
	tmp = ft_strsplit(tmp[0], ' ');
	if (count_lines(tmp) != 2)
		return (0);
	ext = tmp[1];
	tmp = ft_strsplit(tmp[0], '.');
	if (count_lines(tmp) != 2)
		return (0);
	obj->m_env.sprites[i].pnt.x = ft_atoi(tmp[0]) +
		(double)ft_atoi(tmp[1]) / 10;
	tmp = ft_strsplit(ext, '.');
	if (count_lines(tmp) != 2)
		return (0);
	obj->m_env.sprites[i].pnt.y = ft_atoi(tmp[0]) +
		(double)ft_atoi(tmp[1]) / 10;
	i = 0;
	while (tmp[i])
		ft_strdel(&tmp[i++]);
	return (1);
}

int		store_map(t_env *obj, char *str, int x)
{
	char	**tmp;
	int		i;

	obj->m_env.map[x] = (int*)malloc(sizeof(int) * obj->m_env.width);
	if (!obj->m_env.map[x])
		return (0);
	tmp = ft_strsplit(str, ' ');
	if ((int)count_lines(tmp) != obj->m_env.width)
		return (0);
	i = obj->m_env.width;
	while (i-- > 0)
		obj->m_env.map[x][i] = ft_atoi(tmp[i]);
	i = 0;
	while (tmp[i])
		ft_strdel(&tmp[i++]);
	return (1);
}

int		read_map(t_env *obj, int size, int i)
{
	char	*tmp;
	int		check;
	int		fd;

	fd = open(obj->m_env.av, O_RDONLY);
	if (fd < 0)
		return ((int)error(ft_strjoin("Can't open file: ", obj->m_env.av)));
	while (get_next_line(fd, &tmp))
	{
		if (i == 0)
			check = store_info(obj, tmp);
		else if (i > 1 && i < obj->m_env.num_s + 2)
			check = store_sprite(obj, tmp, i - 2);
		else if (i > obj->m_env.num_s + 2 && i < obj->m_env.num_s +
			obj->m_env.height + 3)
			check = store_map(obj, tmp, (i - obj->m_env.num_s - 3));
		if (check == 0)
			return ((int)error(ft_strjoin(obj->m_env.av,
				", has the wrong file format")));
		obj->load_per = RANGE(i++, 65, 100, 0, size);
	}
	if ((i - obj->m_env.num_s - 3) != obj->m_env.height)
		return ((int)error(ft_strjoin(obj->m_env.av,
			", has the wrong file height")));
	return (1);
}
