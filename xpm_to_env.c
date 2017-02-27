/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   xpm_to_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhurt <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/26 21:38:40 by lhurt             #+#    #+#             */
/*   Updated: 2017/02/26 21:38:41 by lhurt            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "xpm.h"

void	dup_map(t_env *obj, t_tex *tmp, int i)
{
	int x;
	int y;

	y = tmp->height;
	while (--y >= 0)
	{
		x = tmp->width;
		while (--x >= 0)
			obj->tex[i][y][x] = tmp->pnts[y][x];
	}
}

void	store_tex_map(t_env *obj, t_tex *tmp, int i)
{
	int	j;

	j = tmp->width;
	obj->tex[i] = (int**)malloc(sizeof(int*) * tmp->height);
	while (--j >= 0)
		obj->tex[i][j] = (int*)malloc(sizeof(int) * tmp->width);
	dup_map(obj, tmp, i);
}

int		get_texture(t_env *obj)
{
	t_tex	*tmp;
	int		i;

	i = -1;
	tmp = malloc(sizeof(t_tex));
	if (!tmp)
		return ((int)error("Malloc failed"));
	if (!get_color_ref(obj, tmp, "colors"))
		return (0);
	if (!get_files(obj, tmp, "files"))
		return (0);
	obj->tex = (int***)malloc(sizeof(int**) * tmp->num_files);
	if (!obj)
		return ((int)error("Malloc failed"));
	while (i++ < tmp->num_files - 1)
	{
		obj->load_per = RANGE(i, 30, 65, 0, tmp->num_files);
		if (!read_xpm(tmp, tmp->files[i], 0))
			return (0);
		store_tex_map(obj, tmp, i);
	}
	fill_pal(obj);
	free(tmp);
	return (1);
}
