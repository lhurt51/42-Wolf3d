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

# include "wolf.h"

typedef struct		s_color
{
	char			*def;
	int				color;
}					t_color;

typedef struct		s_tex
{
	t_color			*color_ref;
	t_color			*color;
	char			**files;
	char			*name;
	int				num_files;
	int				height;
	int				width;
	unsigned int	num_color;
	int				pix_w;
	int				pnts[T_SIZE][T_SIZE];
}					t_tex;

int					charhextoint(char *str, int i);
void				fill_pal(t_env *obj);
int					get_color_ref(t_env *m, t_tex *obj, char *av);
int					get_files(t_env *m, t_tex *obj, char *av);
void				parse_info(t_tex *obj, char *str);
void				store_color(t_tex *obj, char *str, int i, int j);
int					read_xpm(t_tex *obj, char *av, unsigned i);

#endif
