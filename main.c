/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhurt <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/23 18:40:06 by lhurt             #+#    #+#             */
/*   Updated: 2017/01/23 18:40:07 by lhurt            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf.h"

int		worldmap[M_WIDTH][M_HEIGHT] =
{
	{8,8,8,8,8,8,8,8,8,8,8,4,4,6,4,4,6,4,6,4,4,4,6,4},
	{8,0,0,0,0,0,0,0,0,0,8,4,0,0,0,0,0,0,0,0,0,0,0,4},
	{8,0,3,3,0,0,0,0,0,8,8,4,0,0,0,0,0,0,0,0,0,0,0,6},
	{8,0,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6},
	{8,0,3,3,0,0,0,0,0,8,8,4,0,0,0,0,0,0,0,0,0,0,0,4},
	{8,0,0,0,0,0,0,0,0,0,8,4,0,0,0,0,0,6,6,6,0,6,4,6},
	{8,8,8,8,0,8,8,8,8,8,8,4,4,4,4,4,4,6,0,0,0,0,0,6},
	{7,7,7,7,0,7,7,7,7,0,8,0,8,0,8,0,8,4,0,4,0,6,0,6},
	{7,7,0,0,0,0,0,0,7,8,0,8,0,8,0,8,8,6,0,0,0,0,0,6},
	{7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8,6,0,0,0,0,0,4},
	{7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8,6,0,6,0,6,0,6},
	{7,7,0,0,0,0,0,0,7,8,0,8,0,8,0,8,8,6,4,6,0,6,6,6},
	{7,7,7,7,0,7,7,7,7,8,8,4,0,6,8,4,8,3,3,3,0,3,3,3},
	{2,2,2,2,0,2,2,2,2,4,6,4,0,0,6,0,6,3,0,0,0,0,0,3},
	{2,2,0,0,0,0,0,2,2,4,0,0,0,0,0,0,4,3,0,0,0,0,0,3},
	{2,0,0,0,0,0,0,0,2,4,0,0,0,0,0,0,4,3,0,0,0,0,0,3},
	{1,0,0,0,0,0,0,0,1,4,4,4,4,4,6,0,6,3,3,0,0,0,3,3},
	{2,0,0,0,0,0,0,0,2,2,2,1,2,2,2,6,6,0,0,5,0,5,0,5},
	{2,2,0,0,0,0,0,2,2,2,0,0,0,2,2,0,5,0,5,0,0,0,5,5},
	{2,0,0,0,0,0,0,0,2,0,0,0,0,0,2,5,0,5,0,5,0,5,0,5},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5},
	{2,0,0,0,0,0,0,0,2,0,0,0,0,0,2,5,0,5,0,5,0,5,0,5},
	{2,2,0,0,0,0,0,2,2,2,0,0,0,2,2,0,5,0,5,0,0,0,5,5},
	{2,2,2,2,1,2,2,2,2,2,2,1,2,2,2,5,5,5,5,5,5,5,5,5}
};

int		exit_hook(t_mlx *obj)
{
	free(obj);
	exit(0);
}

double	get_time(void)
{
	clock_serv_t	cclock;
	mach_timespec_t	mts;

	host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
	clock_get_time(cclock, &mts);
	mach_port_deallocate(mach_task_self(), cclock);
	return (mts.tv_nsec);
}

void	set_env(t_mlx *obj, t_intpoint *map, int x)
{
	//calculate ray position and direction
	obj->env.vec.ray_dir.x = obj->env.vec.dir.x + obj->env.vec.plane.x * CAMERA_X(x);
	obj->env.vec.ray_dir.y = obj->env.vec.dir.y + obj->env.vec.plane.y * CAMERA_X(x);
	//which box of the map we're in
	map->x = (int)obj->env.vec.pos.x;
	map->y = (int)obj->env.vec.pos.y;
	//length of ray from one x or y-side to next x or y-side
	obj->env.vec.delta_dist.x = DELTADIST(obj->env.vec.ray_dir.x, obj->env.vec.ray_dir.y);
	obj->env.vec.delta_dist.y = DELTADIST(obj->env.vec.ray_dir.y, obj->env.vec.ray_dir.x);
}

void	find_dir_step(t_mlx *obj, t_intpoint *map, t_point *side_dist)
{
	//calculate step and initial sideDist
	if (obj->env.vec.ray_dir.x < 0)
	{
		obj->env.vec.step.x = -1;
		side_dist->x = (obj->env.vec.pos.x - map->x) * obj->env.vec.delta_dist.x;
	}
	else
	{
		obj->env.vec.step.x = 1;
		side_dist->x = (map->x + 1.0 - obj->env.vec.pos.x) * obj->env.vec.delta_dist.x;
	}
	if (obj->env.vec.ray_dir.y < 0)
	{
		obj->env.vec.step.y = -1;
		side_dist->y = (obj->env.vec.pos.y - map->y) * obj->env.vec.delta_dist.y;
	}
	else
	{
		obj->env.vec.step.y = 1;
		side_dist->y = (map->y + 1.0 - obj->env.vec.pos.y) * obj->env.vec.delta_dist.y;
	}
}

void	exe_dda(t_mlx *obj, t_intpoint *map, t_point *side_dist)
{
	int hit; //was there a wall hit?

	hit = 0;
	while (hit == 0)
	{
		//jump to next map square, OR in x-direction, OR in y-direction
		if (side_dist->x < side_dist->y)
		{
			 side_dist->x += obj->env.vec.delta_dist.x;
			 map->x += obj->env.vec.step.x;
			 obj->env.side = 0;
		}
		else
		{
			 side_dist->y += obj->env.vec.delta_dist.y;
			 map->y += obj->env.vec.step.y;
			 obj->env.side = 1;
		}
		//Check if ray has hit a wall
		if (worldmap[map->x][map->y] > 0)
			hit = 1;
	}
}

void	find_draw_pnts(t_mlx *obj, t_intpoint *map)
{
	//Calculate height of line to draw on screen
	if (obj->env.side == 0)
		obj->env.wall_dist = (map->x - obj->env.vec.pos.x + (1 - obj->env.vec.step.x) / 2) / obj->env.vec.ray_dir.x;
	else
		obj->env.wall_dist = (map->y - obj->env.vec.pos.y + (1 - obj->env.vec.step.y) / 2) / obj->env.vec.ray_dir.y;
	obj->env.line_h = (int)(W_HEIGHT * 1 / obj->env.wall_dist);
	//calculate lowest and highest pixel to fill in current stripe
	obj->env.draw_start = -obj->env.line_h / 2 + W_HEIGHT / 2;
	if(obj->env.draw_start < 0)
		obj->env.draw_start = 0;
	obj->env.draw_end = obj->env.line_h / 2 + W_HEIGHT / 2;
	if(obj->env.draw_end  >= W_HEIGHT)
		obj->env.draw_end  = W_HEIGHT - 1;
}

void	get_fps(t_mlx *obj)
{
	double	old_time;
	char	*print;
	char	*tmp;

	old_time = obj->env.cur_time;
	obj->env.cur_time = get_time();
	if (obj->env.cur_time < old_time)
		obj->env.cur_time += 1000000000;
	obj->env.frame_time = (double)(obj->env.cur_time - old_time) / 1000000000.0;
	print = ft_strnew(5);
	print = "FPS: ";
	tmp = ft_itoa((int)1.0 / obj->env.frame_time);
	print = ft_strjoin(print, tmp);
	mlx_string_put(obj->mlx, obj->win, 0, 0, 0xFFFFFF, print);
}

void	choose_color(t_mlx *obj, t_intpoint *map)
{
	int color;

	//choose wall color
	switch (worldmap[map->x][map->y])
	{
		case 1:
				color = 0xFF0000;
				break; //red
		case 2:
				color = 0x00FF00;
				break; //green
		case 3:
				color = 0x0000FF;
				break; //blue
		case 4:
				color = 0xFFFFFF;
				break; //white
		default:
				color = 0xFFFF00;
				break; //yellow
	}
	//give x and y sides different brightness
	if (obj->env.side == 1)
		color = color / 2;
	//draw the pixels of the stripe as a vertical line
	obj->env.color = color;
}

void	choose_tex(t_mlx *obj, t_intpoint *map, int x)
{
	int 	tex_num;
	int		tex_x;

	tex_num = worldmap[map->x][map->y] + 2;
	if (obj->env.side == 0)
		obj->env.wall_x = obj->env.vec.pos.y + obj->env.wall_dist * obj->env.vec.ray_dir.y;
	else
		obj->env.wall_x = obj->env.vec.pos.x + obj->env.wall_dist * obj->env.vec.ray_dir.x;
	obj->env.wall_x -= floor(obj->env.wall_x);

	tex_x = (int)(obj->env.wall_x * (double)(T_SIZE));
	if (obj->env.side == 0 && obj->env.vec.ray_dir.x > 0)
		tex_x = T_SIZE - tex_x - 1;
	if (obj->env.side == 1 && obj->env.vec.ray_dir.y < 0)
		tex_x = T_SIZE - tex_x - 1;

	int ran;
	int tex_y;
	int	color;
	int y;

	y = obj->env.draw_start;
	if (tex_num > 0)
		tex_num += tex_num;
	while (y < obj->env.draw_end)
	{
		ran = y * 256 - W_HEIGHT * 128 + obj->env.line_h * 128;
		tex_y = ((ran * T_SIZE) / obj->env.line_h) / 256;
		color = obj->env.tex[tex_num][tex_y][tex_x];
		if (obj->env.side == 1)
			color = obj->env.tex[tex_num + 1][tex_y][tex_x];
		if ((x < W_WIDTH && x > 0) && (y < W_HEIGHT && y > 0))
			pixel_to_img(obj, x, y, color);
		y++;
	}
}

void	draw_floor(t_mlx *obj, t_point *floor_wall, int x)
{
	t_point		cur_floor;
	t_intpoint	floor_tex;
	double		dist_wall;
	double		dist_player;
	double		cur_dist;
	double		weight;
	int			color;
	int			y;

	dist_wall = obj->env.wall_dist;
	dist_player = 0.0;
	if (obj->env.draw_end < 0)
		obj->env.draw_end = W_HEIGHT;
	y = obj->env.draw_end + 1;
	while (y < W_HEIGHT)
	{
		cur_dist = W_HEIGHT / (2.0 * y - W_HEIGHT);
		weight = (cur_dist - dist_player) / (dist_wall - dist_player);
		cur_floor.x = weight * floor_wall->x + (1.0 - weight) * obj->env.vec.pos.x;
		cur_floor.y = weight * floor_wall->y + (1.0 - weight) * obj->env.vec.pos.y;
		floor_tex.x = (int)(cur_floor.x * T_SIZE) % T_SIZE;
		floor_tex.y = (int)(cur_floor.y * T_SIZE) % T_SIZE;
		color = obj->env.tex[0][floor_tex.y][floor_tex.x];
		if ((x < W_WIDTH && x > 0) && (y < W_HEIGHT && y > 0))
			pixel_to_img(obj, x, y, color);
		color = obj->env.tex[22][floor_tex.y][floor_tex.x];
		if ((x < W_WIDTH && x > 0) && (W_HEIGHT - y < W_HEIGHT && W_HEIGHT - y > 0))
			pixel_to_img(obj, x, W_HEIGHT - y, color);
		y++;
	}
}


void	floor_casting(t_mlx *obj, t_intpoint *map, int x)
{
	t_point		floor_wall;

	if (obj->env.side == 0 && obj->env.vec.ray_dir.x > 0)
	{
		floor_wall.x = map->x;
		floor_wall.y = map->y + obj->env.wall_x;
	}
	else if (obj->env.side == 0 && obj->env.vec.ray_dir.x < 0)
	{
		floor_wall.x = map->x + 1.0;
		floor_wall.y = map->y + obj->env.wall_x;
	}
	else if (obj->env.side == 1 && obj->env.vec.ray_dir.y > 0)
	{
		floor_wall.x = map->x + obj->env.wall_x;
		floor_wall.y = map->y;
	}
	else
	{
		floor_wall.x = map->x + obj->env.wall_x;
		floor_wall.y = map->y + 1.0;
	}
	draw_floor(obj, &floor_wall, x);
}

void	init_rays(t_mlx *obj)
{
	int 		x;
	t_point		side_dist;
	t_intpoint	map;

	x = W_WIDTH;
	while (x > 0)
	{
		set_env(obj, &map, x);
		find_dir_step(obj, &map, &side_dist);
		exe_dda(obj, &map, &side_dist);
		find_draw_pnts(obj, &map);
		choose_tex(obj, &map, x);
		floor_casting(obj, &map, x);
		x--;
	}
}

void	reset_struct(t_mlx *obj)
{
	obj->env.vec.pos.x = 22.0;
	obj->env.vec.pos.y = 11.5;
	obj->env.vec.dir.x = -1.0;
	obj->env.vec.dir.y = 0.0;
	obj->env.vec.plane.x = 0.0;
	obj->env.vec.plane.y = 0.66;
}

void		draw_crosshair(t_mlx *obj)
{
	int	x;
	int	y;

	x = X_ORIGIN + CH_LEN;
	while (x >= X_ORIGIN - CH_LEN)
	{
		y = Y_ORIGIN + CH_WID;
		while (y >= Y_ORIGIN - CH_WID)
		{
			if (x > X_ORIGIN + CH_OFF || x < X_ORIGIN - CH_OFF)
				mlx_pixel_put(obj->mlx, obj->win, x, y, CH_COLOR);
			y--;
		}
		x--;
	}
	y = Y_ORIGIN + CH_LEN;
	while (y >= Y_ORIGIN - CH_LEN)
	{
		x = X_ORIGIN + CH_WID;
		while (x >= X_ORIGIN - CH_WID)
		{
			if (y > Y_ORIGIN + CH_OFF || y < Y_ORIGIN - CH_OFF)
				mlx_pixel_put(obj->mlx, obj->win, x, y, CH_COLOR);
			x--;
		}
		y--;
	}
}

int		run_img(t_mlx *obj)
{
	obj->env.cur_time = get_time();
	init_rays(obj);
	mlx_put_image_to_window(obj->mlx, obj->win, obj->img, 0, 0);
	get_fps(obj);
	draw_crosshair(obj);
	return (0);
}

void	move_up(t_mlx *obj)
{
	double	move_speed;

	move_speed = obj->env.frame_time * 20.0;
	if(worldmap[(int)(obj->env.vec.pos.x + obj->env.vec.dir.x * move_speed)][(int)obj->env.vec.pos.y] == 0)
		obj->env.vec.pos.x += obj->env.vec.dir.x * move_speed;
	if(worldmap[(int)obj->env.vec.pos.x][(int)(obj->env.vec.pos.y + obj->env.vec.dir.y * move_speed)] == 0)
		obj->env.vec.pos.y += obj->env.vec.dir.y * move_speed;
}

void	move_down(t_mlx *obj)
{
	double	move_speed;

	move_speed = obj->env.frame_time * 20.0;
	if(worldmap[(int)(obj->env.vec.pos.x - obj->env.vec.dir.x * move_speed)][(int)obj->env.vec.pos.y] == 0)
		obj->env.vec.pos.x -= obj->env.vec.dir.x * move_speed;
	if(worldmap[(int)obj->env.vec.pos.x][(int)(obj->env.vec.pos.y - obj->env.vec.dir.y * move_speed)] == 0)
		obj->env.vec.pos.y -= obj->env.vec.dir.y * move_speed;
}

void	rot_right(t_mlx *obj)
{
	double	old_dirx;
	double	old_planex;
	double	rot_speed;

	rot_speed = obj->env.frame_time * (M_PI * 2);
	old_dirx = obj->env.vec.dir.x;
	old_planex = obj->env.vec.plane.x;
	obj->env.vec.dir.x = obj->env.vec.dir.x * cos(-rot_speed) - obj->env.vec.dir.y * sin(-rot_speed);
	obj->env.vec.dir.y = old_dirx * sin(-rot_speed) + obj->env.vec.dir.y * cos(-rot_speed);
	obj->env.vec.plane.x = obj->env.vec.plane.x * cos(-rot_speed) - obj->env.vec.plane.y * sin(-rot_speed);
	obj->env.vec.plane.y = old_planex * sin(-rot_speed) + obj->env.vec.plane.y * cos(-rot_speed);
}

void	rot_left(t_mlx *obj)
{
	double	old_dirx;
	double	old_planex;
	double	rot_speed;

	rot_speed = obj->env.frame_time * (M_PI * 2);
	old_dirx = obj->env.vec.dir.x;
	old_planex = obj->env.vec.plane.x;
	obj->env.vec.dir.x = obj->env.vec.dir.x * cos(rot_speed) - obj->env.vec.dir.y * sin(rot_speed);
	obj->env.vec.dir.y = old_dirx * sin(rot_speed) + obj->env.vec.dir.y * cos(rot_speed);
	obj->env.vec.plane.x = obj->env.vec.plane.x * cos(rot_speed) - obj->env.vec.plane.y * sin(rot_speed);
	obj->env.vec.plane.y = old_planex * sin(rot_speed) + obj->env.vec.plane.y * cos(rot_speed);
}

int		my_key_press(int keycode, t_mlx *obj)
{
	// printf("Keycode: %d\n", keycode);
	if (keycode == 53)
		exit_hook(obj);
	else if (keycode == 0)
		rot_left(obj);
	else if (keycode == 1)
		move_down(obj);
	else if(keycode == 2)
		rot_right(obj);
	else if (keycode == 13)
		move_up(obj);
	return (0);
}

void	run_win(t_mlx *obj)
{
	obj->mlx = mlx_init();
	obj->win = mlx_new_window(obj->mlx, W_WIDTH, W_HEIGHT, "Wolf3D");
	if (obj->img)
		mlx_destroy_image(obj->mlx, obj->img);
	obj->img = mlx_new_image(obj->mlx, W_WIDTH, W_HEIGHT);
	obj->data = mlx_get_data_addr(obj->img, &obj->bits, &obj->size_line,
		&obj->endian);
	// run_img(obj);
	// mlx_mouse_hook(obj->win, my_mouse_func, obj);
	// mlx_hook(obj->win, 6, 0, my_mouse_movement, obj);
	mlx_hook(obj->win, 2, 0, my_key_press, obj);
	mlx_hook(obj->win, 17, 0, exit_hook, obj);
	mlx_loop_hook(obj->mlx, run_img, obj);
	mlx_loop(obj->mlx);
}

int		main()
{
	t_mlx	*obj;

	obj = malloc(sizeof(t_mlx));
	if(!obj)
		return ((int)error("Malloc failed"));
	reset_struct(obj);
	get_texture(obj);
	run_win(obj);
	free(obj);
	return(0);
}
