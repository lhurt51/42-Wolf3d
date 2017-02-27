/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tex_pallet.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhurt <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/26 21:36:40 by lhurt             #+#    #+#             */
/*   Updated: 2017/02/26 21:36:41 by lhurt            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "xpm.h"

void	fill_pal2(t_env *obj)
{
	obj->tex_pal[12][0] = 18;
	obj->tex_pal[12][1] = 17;
	obj->tex_pal[13][0] = 16;
	obj->tex_pal[13][1] = 19;
	obj->tex_pal[14][0] = 20;
	obj->tex_pal[14][1] = 17;
	obj->tex_pal[15][0] = 16;
	obj->tex_pal[15][1] = 21;
	obj->tex_pal[16][0] = 22;
	obj->tex_pal[16][1] = 17;
	obj->tex_pal[17][0] = 16;
	obj->tex_pal[17][1] = 23;
	obj->tex_pal[18][0] = 24;
	obj->tex_pal[18][1] = 17;
	obj->tex_pal[19][0] = 16;
	obj->tex_pal[19][1] = 25;
	obj->tex_pal[20][0] = 26;
	obj->tex_pal[20][1] = 17;
	obj->tex_pal[21][0] = 16;
	obj->tex_pal[21][1] = 27;
	obj->tex_pal[22][0] = 28;
	obj->tex_pal[22][1] = 29;
	obj->tex_pal[23][0] = 30;
	obj->tex_pal[23][1] = 31;
}

void	fill_pal(t_env *obj)
{
	obj->tex_pal[0][0] = 4;
	obj->tex_pal[0][1] = 5;
	obj->tex_pal[1][0] = 6;
	obj->tex_pal[1][1] = 7;
	obj->tex_pal[2][0] = 6;
	obj->tex_pal[2][1] = 5;
	obj->tex_pal[3][0] = 4;
	obj->tex_pal[3][1] = 7;
	obj->tex_pal[4][0] = 8;
	obj->tex_pal[4][1] = 5;
	obj->tex_pal[5][0] = 4;
	obj->tex_pal[5][1] = 9;
	obj->tex_pal[6][0] = 10;
	obj->tex_pal[6][1] = 5;
	obj->tex_pal[7][0] = 4;
	obj->tex_pal[7][1] = 11;
	obj->tex_pal[8][0] = 12;
	obj->tex_pal[8][1] = 13;
	obj->tex_pal[9][0] = 15;
	obj->tex_pal[9][1] = 13;
	obj->tex_pal[10][0] = 12;
	obj->tex_pal[10][1] = 14;
	obj->tex_pal[11][0] = 16;
	obj->tex_pal[11][1] = 17;
	fill_pal2(obj);
}
