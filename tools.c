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

unsigned int	count_ord(char *av)
{
	int				fd;
	char			*tmp;
	unsigned int	count;

	count = 0;
	fd = open(av, O_RDONLY);
	if (fd < 0)
		return ((int)error("failed to open file"));
	while (get_next_line(fd, &tmp))
		count++;
	close(fd);
	return (count);
}

void	pixel_to_img(t_mlx *new, int x, int y, int color)
{
	int		i;

	i = (x * 4) + (y * new->size_line);
	new->data[i++] = color & 0xFF;
	new->data[i++] = (color >> 8) & 0xFF;
	new->data[i] = (color >> 16) & 0xFF;
}
