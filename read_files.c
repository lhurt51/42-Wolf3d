/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_files.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhurt <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/26 20:51:15 by lhurt             #+#    #+#             */
/*   Updated: 2017/02/26 20:51:18 by lhurt            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "xpm.h"

int		store_num(t_tex *obj, char *str)
{
	obj->num_files = atoi(str);
	if (obj->num_files < 0)
		return (0);
	obj->files = (char**)malloc(sizeof(char*) * obj->num_files);
	if (!obj->files)
		return (0);
	return (1);
}

int		store_files(t_tex *obj, char *tmp, int i)
{
	char	**str;

	tmp = ft_strtrim(tmp, '"');
	str = ft_strsplit(tmp, ' ');
	if (count_lines(str) > 1)
		return (0);
	obj->files[i] = ft_strdup(str[0]);
	ft_memdel((void**)str);
	return (1);
}

int		get_files(t_env *m, t_tex *obj, char *av)
{
	int				fd;
	int				i;
	int				check;
	char			*tmp;

	i = 0;
	fd = open(av, O_RDONLY);
	if (fd < 0)
		return ((int)error(ft_strjoin("Can't open file: ", av)));
	while (get_next_line(fd, &tmp))
	{
		if (i == 0)
			check = store_num(obj, tmp);
		else if (i > 1)
			check = store_files(obj, tmp, i - 2);
		if (!check)
			return ((int)error(ft_strjoin(av, ", has the wrong file format")));
		m->load_per = RANGE(i, 20, 30, 0, obj->num_files);
		i++;
	}
	close(fd);
	return (1);
}
