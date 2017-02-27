/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_colors.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhurt <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/26 18:39:57 by lhurt             #+#    #+#             */
/*   Updated: 2017/02/26 18:39:59 by lhurt            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "xpm.h"

void	str_upper(char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (ft_islower(str[i]))
			str[i] = ft_toupper(str[i]);
		i++;
	}
}

int		get_color_ref(t_env *m, t_tex *obj, char *av)
{
	int				fd;
	int				i;
	char			*tmp;
	char			**str;

	i = 0;
	obj->color_ref = (t_color*)malloc(sizeof(t_color) * 754);
	if (!obj->color_ref)
		return ((int)error("Malloc failed"));
	fd = open(av, O_RDONLY);
	if (fd < 0)
		return ((int)error(ft_strjoin("Can't open file: ", av)));
	while (get_next_line(fd, &tmp))
	{
		m->load_per = RANGE(i, 0, 20, 0, 754);
		str = ft_strsplit(tmp, '"');
		if (count_lines(str) != 2)
			return ((int)error(ft_strjoin(av, ", has the wrong file format")));
		obj->color_ref[i].def = ft_strdup(str[0]);
		str_upper(&str[1][2]);
		obj->color_ref[i++].color = charhextoint(&str[1][2],
			ft_strlen(&str[1][2]) - 1);
	}
	close(fd);
	return (1);
}
