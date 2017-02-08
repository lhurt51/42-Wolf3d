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

void	pixel_to_img(t_mlx *new, int x, int y, int color)
{
	int		i;

	i = (x * 4) + (y * new->size_line);
	if (i > 0 && color >= 0)
	{
		new->data[i++] = color & 0xFF;
		new->data[i++] = (color >> 8) & 0xFF;
		new->data[i] = (color >> 16) & 0xFF;
	}
}
