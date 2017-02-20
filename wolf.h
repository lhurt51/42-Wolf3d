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

# include <stdio.h>
# include <time.h>
# include <mach/clock.h>
# include <mach/mach.h>

# define W_HEIGHT 720
# define W_WIDTH 1280
# define T_FILES 36
# define T_SIZE 64
# define ROT_BUFF (X_ORIGIN / 6)
# define MOVE_BUFF 1.75
# define CH_LEN 15
# define CH_WID 1
# define CH_OFF 4
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

typedef struct		s_sprite
{
	t_point			pnt;
	int				tex;
}					t_sprite;

typedef struct		s_pressed
{
	int				m_left;
	int				m_right;
	int				m_1;
	int				shift;
	int				a;
	int				d;
	int				w;
	int				s;
}					t_pressed;

typedef struct		s_vec
{
	t_point			pos; // x & y starting pos
	t_point			dir; // starting direction vectors
	t_point			plane; //the 2d raycaster version of camera plane
	t_point			ray_dir;  //calculate ray position and direction
	t_point			delta_dist; //length of ray from one x or y-side to next x or y-side
	t_point			step; //what direction to step in x or y-direction (either +1 or -1)
}					t_vec;

typedef struct		s_mlx
{
	t_pressed		keys;
	void			*mlx;
	void			*win;
	void			*img;
	char			*data;
	int				bits;
	int				size_line;
	int				endian;
	double			m_off;
}					t_mlx;

typedef struct		s_var
{
	double			frame_time; //time of the frame
	double			cur_time;
	double			wall_dist;
	double			wall_x;
	double			sprint;
	int				line_h;
	int				side; //was a NS or a EW wall hit?
	int				draw_start;
	int				draw_end;
	int				color;
}					t_var;

typedef struct		s_m_env
{
	t_sprite		*sprites;
	int				**map;
	int				num_s;
	int				height;
	int				width;
}					t_m_env;

typedef struct		s_env
{
	t_vec			vec;
	t_mlx			mlx;
	t_var			var;
	t_m_env			m_env;
	int				**tex[T_FILES];
	double			z_buff[W_WIDTH + 1];
}					t_env;

void			*error(char *msg);
int				exit_hook(t_env *obj);
unsigned int	count_lines(char **str);
void			swap_mem(double *dst, double *src);
void			swap_memin(int *dst, int *src);
double			get_time(void);
void			run_keys(t_env *obj);
int				my_key_press(int keycode, t_env *obj);
int				my_key_release(int keycode, t_env *obj);
void 			run_mouse(t_env *obj);
int				my_mouse_movement(int x, int y, t_env *obj);
int				run_img(t_env *obj);
void			pixel_to_img(t_env *new, int x, int y, int color);
int				get_texture(t_env *obj);

#endif
