/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wolf.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhurt <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/23 18:40:17 by lhurt             #+#    #+#             */
/*   Updated: 2017/01/23 18:40:20 by lhurt            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WOLF_H
# define WOLF_H

# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <math.h>
# include <mlx.h>
# include "libft/libft.h"

void			*error(char *msg);
unsigned int	count_ord(char *av);
void			pixel_to_img(t_mlx *new, int x, int y, int color);

#endif
