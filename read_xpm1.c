/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_xpm1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhurt <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/26 21:42:31 by lhurt             #+#    #+#             */
/*   Updated: 2017/02/26 21:42:32 by lhurt            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "xpm.h"

void	parse_info(t_tex *obj, char *str)
{
	char			**tmp;
	unsigned int	i;

	tmp = ft_strsplit(str, ' ');
	obj->width = atoi(tmp[0]);
	obj->height = atoi(tmp[1]);
	obj->num_color = atoi(tmp[2]);
	obj->pix_w = atoi(tmp[3]);
	obj->color = (t_color*)malloc(sizeof(t_color) * obj->num_color);
	i = obj->num_color;
	while (i-- > 0)
		obj->color[i].def = ft_strnew(obj->pix_w);
	i = 0;
	while (tmp[i])
		ft_strdel(&tmp[i++]);
}

void	find_xt_color(t_tex *obj, char *str, int i)
{
	int in;

	in = 753;
	while (--in >= 0)
		if (ft_strequ(str, obj->color_ref[in].def))
			obj->color[i].color = obj->color_ref[in].color;
}

void	store_color(t_tex *obj, char *str, int i, int j)
{
	char	tmp[7];
	int		in;

	in = 0;
	if (str[j] != '#' && in == 0)
		return (find_xt_color(obj, &str[j], i));
	while (str[j++])
		if ((ft_isdigit(str[j]) || ft_isupper(str[j])) && in < 6)
			tmp[in++] = str[j];
	tmp[in] = '\0';
	obj->color[i].color = charhextoint(tmp, ft_strlen(tmp) - 1);
}
