/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_xmp.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhurt <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/15 14:40:22 by lhurt             #+#    #+#             */
/*   Updated: 2017/02/15 14:40:25 by lhurt            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "xpm.h"

void	parse_color(t_tex *obj, char *str, int i)
{
	int	j;

	j = obj->pix_w;
	while (--j >= 0)
		obj->color[i].def[j] = str[j];
	j = obj->pix_w + 3;
	store_color(obj, str, i, j);
}

int		check_colors(t_tex *obj, char *str)
{
	unsigned int	i;

	i = obj->num_color;
	while (i-- > 0)
		if (ft_strnequ(obj->color[i].def, str, obj->pix_w))
			return (obj->color[i].color);
	return (-1);
}

void	fill_board(t_tex *obj, char *str, int *row)
{
	char	*tmp;
	int		col;
	int		i;

	i = 0;
	col = 0;
	tmp = ft_strnew(obj->pix_w);
	while (i < obj->width * obj->pix_w && col < obj->width)
	{
		tmp[i % obj->pix_w] = str[i];
		if (i % obj->pix_w == 1)
			obj->pnts[*row][col++] = check_colors(obj, tmp);
		i++;
	}
	ft_strdel(&tmp);
}

int		read_xpm(t_tex *obj, char *av, unsigned i)
{
	int				fd;
	int				y;
	char			*tmp;

	y = -1;
	fd = open(av, O_RDONLY);
	if (fd < 0)
		return ((int)error(ft_strjoin("Can't open file: ", av)));
	while (get_next_line(fd, &tmp))
	{
		tmp = ft_strtrim(tmp, ',');
		tmp = ft_strtrim(tmp, '"');
		if (i == 3)
			parse_info(obj, tmp);
		else if (i > 3 && i < obj->num_color + 4)
			parse_color(obj, tmp, i - 4);
		else if ((i > obj->num_color + 4) && (i <= obj->num_color +
				obj->height + 4) && (++y <= obj->height))
			fill_board(obj, tmp, &y);
		i++;
	}
	close(fd);
	if (obj->height != T_SIZE || obj->width != T_SIZE)
		return ((int)error(ft_strjoin("XPM wronge size", av)));
	return (1);
}
