/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   xpm.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhurt <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/15 14:40:02 by lhurt             #+#    #+#             */
/*   Updated: 2017/02/15 14:40:05 by lhurt            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef XPM_H
# define XPM_H

typedef struct		s_color
{
	char			*def;
	int				color;
}					t_color;

typedef struct		s_tex
{
	t_color			*color;
	char			*name;
	int				height;
	int				width;
	unsigned int	num_color;
	int				pix_w;
	int				pnts[T_SIZE][T_SIZE];
}					t_tex;

int				charhextoint(char *str, int i);

#endif
