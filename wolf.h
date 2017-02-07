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

#include <stdio.h>
#include <time.h>
#include <mach/clock.h>
#include <mach/mach.h>

# define M_HEIGHT 24
# define M_WIDTH 24
# define W_HEIGHT 720
# define W_WIDTH 1280
# define NUM_FILES 23
# define T_SIZE 64
# define MOVE_BUFF (X_ORIGIN / 8)
# define CH_LEN 15
# define CH_WID 1
# define CH_OFF 2
# define CH_COLOR 0xFFFFFF
# define Y_ORIGIN (W_HEIGHT / 2)
# define X_ORIGIN (W_WIDTH / 2)
# define CAMERA_X(x) (2 * x / (double)(W_WIDTH) - 1) //x-coordinate in camera space
# define DELTADIST(primary, secondary) (sqrt(1 + (secondary * secondary) / (primary * primary)))

typedef struct		s_point
{
	double			x;
	double			y;
}					t_point;

typedef struct		s_intpoint
{
	int				x;
	int				y;
}					t_intpoint;

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

typedef struct		s_line
{
	int				inc;
	float			yans;
	float			xans;
}					t_line;

typedef struct		s_vec
{
	t_point			pos; // x & y starting pos
	t_point			dir; // starting direction vectors
	t_point			plane; //the 2d raycaster version of camera plane
	t_point			ray_dir;  //calculate ray position and direction
	t_point			delta_dist; //length of ray from one x or y-side to next x or y-side
	t_point			step; //what direction to step in x or y-direction (either +1 or -1)
}					t_vec;

typedef struct		s_env
{
	int				**tex[NUM_FILES];
	t_vec			vec;
	double			frame_time; //time of the frame
	double			cur_time;
	double			wall_dist;
	double			wall_x;
	int				line_h;
	int				side; //was a NS or a EW wall hit?
	int				draw_start;
	int				draw_end;
	int				color;
}					t_env;

typedef struct		s_mlx
{
	t_env			env;
	void			*mlx;
	void			*win;
	void			*img;
	char			*data;
	int				bits;
	int				size_line;
	int				endian;
}					t_mlx;

void			*error(char *msg);
unsigned int	count_ord(char *av);
void			pixel_to_img(t_mlx *new, int x, int y, int color);
void			get_da(t_mlx *lst, t_intpoint *point1, t_intpoint *point2);
int				get_texture(t_mlx *obj);

#endif
