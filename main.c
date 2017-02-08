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

t_sprite	sprites[T_SPRITES] =
{
	{{20.5, 11.5}, 23},
	{{18.5, 4.5}, 23},
	{{10.0, 4.5}, 23},
	{{10.0, 12.5}, 23},
	{{3.5, 6.5}, 23},
	{{3.5, 20.5}, 23},
	{{14.5, 20.5}, 23},
	{{18.5, 10.5}, 24},
	{{18.5, 11.5}, 24},
	{{18.5, 12.5}, 24}
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

void	set_env(t_mlx *obj, t_point *map, int x)
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

void	find_dir_step(t_mlx *obj, t_point *map, t_point *side_dist)
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

void	exe_dda(t_mlx *obj, t_point *map, t_point *side_dist)
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
		if (worldmap[(int)map->x][(int)map->y] > 0)
			hit = 1;
	}
}

void	find_draw_pnts(t_mlx *obj, t_point *map)
{
	//Calculate height of line to draw on screen
	if (obj->env.side == 0)
		obj->env.wall_dist = (map->x - obj->env.vec.pos.x + (1 - obj->env.vec.step.x) / 2) / obj->env.vec.ray_dir.x;
	else
		obj->env.wall_dist = (map->y - obj->env.vec.pos.y + (1 - obj->env.vec.step.y) / 2) / obj->env.vec.ray_dir.y;
	obj->env.line_h = (int)(W_HEIGHT / obj->env.wall_dist);
	//calculate lowest and highest pixel to fill in current stripe
	obj->env.draw_start = (-obj->env.line_h) / 2 + W_HEIGHT / 2;
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
	ft_strdel(&tmp);
	ft_strdel(&print);
}

void	choose_tex(t_mlx *obj, t_point *map, int x)
{
	int 	tex_num;
	int		tex_x;

	tex_num = worldmap[(int)map->x][(int)map->y] + 2;
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

	y = (int)obj->env.draw_start;
	if (tex_num > 0)
		tex_num += tex_num;
	while (y < (int)obj->env.draw_end)
	{
		ran = y * 256 - W_HEIGHT * 128 + obj->env.line_h * 128;
		tex_y = abs(((ran * T_SIZE) / obj->env.line_h) / 256);
		if (obj->env.side == 1)
			color = obj->env.tex[tex_num + 1][tex_y][tex_x];
		else// if ((tex_x >= 0 && tex_x < T_SIZE) && (tex_y >= 0 && tex_y < T_SIZE))
			color = obj->env.tex[tex_num][tex_y][tex_x];
		if ((x < W_WIDTH && x >= 0) && (y < W_HEIGHT && y >= 0))
			pixel_to_img(obj, x, y, color);
		y++;
	}
}

void	draw_floor(t_mlx *obj, t_point *floor_wall, int x)
{
	t_point		cur_floor;
	t_point		floor_tex;
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
		color = obj->env.tex[0][(int)floor_tex.y][(int)floor_tex.x];
		if ((x < W_WIDTH && x >= 0) && (y < W_HEIGHT && y >= 0))
			pixel_to_img(obj, x, y, color);
		color = obj->env.tex[22][(int)floor_tex.y][(int)floor_tex.x];
		if ((x < W_WIDTH && x >= 0) && (W_HEIGHT - y < W_HEIGHT && W_HEIGHT - y >= 0))
			pixel_to_img(obj, x, W_HEIGHT - y, color);
		y++;
	}
}


void	floor_casting(t_mlx *obj, t_point *map, int x)
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
	t_point		map;

	x = W_WIDTH;
	while (x > 0)
	{
		set_env(obj, &map, x);
		find_dir_step(obj, &map, &side_dist);
		exe_dda(obj, &map, &side_dist);
		find_draw_pnts(obj, &map);
		choose_tex(obj, &map, x);
		obj->env.z_buff[x] = obj->env.wall_dist;
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
	obj->env.wall_x = 0;
	obj->keys.m_left = 0;
	obj->keys.m_right = 0;
	obj->keys.shift = 0;
	obj->keys.a = 0;
	obj->keys.s = 0;
	obj->keys.d = 0;
	obj->keys.w = 0;
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

void	swap_mem(double *dst, double *src)
{
	double	tmp;

	tmp = *src;
	*src = *dst;
	*dst = tmp;
}

void	swap_memin(int *dst, int *src)
{
	int	tmp;

	tmp = *src;
	*src = *dst;
	*dst = tmp;
}

void	sort_sprites(int *ord, double *dist, int amount)
{
	int	i;
	int	j;
	int ans;
	int swap;

	i = 0;
	swap = 0;
	ans = amount;
	while (ans > 1 || swap)
	{
		ans = (ans * 10) / 13;
		if (ans == 9 || ans == 10)
			ans = 11;
		if (ans < 1)
			ans = 1;
		swap = 0;
		while (i < amount - ans)
		{
			j = i + ans;
			if (dist[i] < dist[j])
			{
				swap_mem(&dist[i], &dist[j]);
				swap_memin(&ord[i], &ord[j]);
				swap = 1;
			}
			i++;
		}
	}
}

void	handle_sprites(t_mlx *obj)
{
	int		sprite_ord[T_SPRITES];
	double	sprite_dis[T_SPRITES];
	int		i;

	i = 0;
	while (i < T_SPRITES)
	{
		sprite_ord[i] = i;
		sprite_dis[i] = ((obj->env.vec.pos.x - sprites[i].pnt.x) * (obj->env.vec.pos.x  - sprites[i].pnt.x) + (obj->env.vec.pos.y - sprites[i].pnt.y) * (obj->env.vec.pos.y - sprites[i].pnt.y));
		i++;
	}
	sort_sprites(sprite_ord, sprite_dis, T_SPRITES);
}

void	move_up(t_mlx *obj)
{
	double	move_speed;

	move_speed = obj->env.frame_time * (2.25 + obj->env.sprint);
	if(worldmap[(int)(obj->env.vec.pos.x + obj->env.vec.dir.x * move_speed)][(int)obj->env.vec.pos.y] == 0)
		obj->env.vec.pos.x += obj->env.vec.dir.x * move_speed;
	if(worldmap[(int)obj->env.vec.pos.x][(int)(obj->env.vec.pos.y + obj->env.vec.dir.y * move_speed)] == 0)
		obj->env.vec.pos.y += obj->env.vec.dir.y * move_speed;
}

void	move_down(t_mlx *obj)
{
	double	move_speed;

	move_speed = obj->env.frame_time * (2.25 + obj->env.sprint);
	if(worldmap[(int)(obj->env.vec.pos.x - obj->env.vec.dir.x * move_speed)][(int)obj->env.vec.pos.y] == 0)
		obj->env.vec.pos.x -= obj->env.vec.dir.x * move_speed;
	if(worldmap[(int)obj->env.vec.pos.x][(int)(obj->env.vec.pos.y - obj->env.vec.dir.y * move_speed)] == 0)
		obj->env.vec.pos.y -= obj->env.vec.dir.y * move_speed;
}

void	move_right(t_mlx *obj)
{
	double	move_speed;

	move_speed = obj->env.frame_time * (2.0 + obj->env.sprint);
	if(worldmap[(int)(obj->env.vec.pos.x + obj->env.vec.plane.x * move_speed)][(int)obj->env.vec.pos.y] == 0)
		obj->env.vec.pos.x += obj->env.vec.plane.x * move_speed;
	if(worldmap[(int)obj->env.vec.pos.x][(int)(obj->env.vec.pos.y + obj->env.vec.plane.y * move_speed)] == 0)
		obj->env.vec.pos.y += obj->env.vec.plane.y * move_speed;
}

void	move_left(t_mlx *obj)
{
	double	move_speed;

	move_speed = obj->env.frame_time * (2.0 + obj->env.sprint);
	if(worldmap[(int)(obj->env.vec.pos.x - obj->env.vec.plane.x * move_speed)][(int)obj->env.vec.pos.y] == 0)
		obj->env.vec.pos.x -= obj->env.vec.plane.x * move_speed;
	if(worldmap[(int)obj->env.vec.pos.x][(int)(obj->env.vec.pos.y - obj->env.vec.plane.y * move_speed)] == 0)
		obj->env.vec.pos.y -= obj->env.vec.plane.y * move_speed;
}

void	rot_right(t_mlx *obj, double off)
{
	double	old_dirx;
	double	old_planex;
	double	rot_speed;

	rot_speed = obj->env.frame_time * (M_PI * (0.125 + off));
	old_dirx = obj->env.vec.dir.x;
	old_planex = obj->env.vec.plane.x;
	obj->env.vec.dir.x = obj->env.vec.dir.x * cos(-rot_speed) - obj->env.vec.dir.y * sin(-rot_speed);
	obj->env.vec.dir.y = old_dirx * sin(-rot_speed) + obj->env.vec.dir.y * cos(-rot_speed);
	obj->env.vec.plane.x = obj->env.vec.plane.x * cos(-rot_speed) - obj->env.vec.plane.y * sin(-rot_speed);
	obj->env.vec.plane.y = old_planex * sin(-rot_speed) + obj->env.vec.plane.y * cos(-rot_speed);
}

void	rot_left(t_mlx *obj, double off)
{
	double	old_dirx;
	double	old_planex;
	double	rot_speed;

	rot_speed = obj->env.frame_time * (M_PI * (0.125 + off));
	old_dirx = obj->env.vec.dir.x;
	old_planex = obj->env.vec.plane.x;
	obj->env.vec.dir.x = obj->env.vec.dir.x * cos(rot_speed) - obj->env.vec.dir.y * sin(rot_speed);
	obj->env.vec.dir.y = old_dirx * sin(rot_speed) + obj->env.vec.dir.y * cos(rot_speed);
	obj->env.vec.plane.x = obj->env.vec.plane.x * cos(rot_speed) - obj->env.vec.plane.y * sin(rot_speed);
	obj->env.vec.plane.y = old_planex * sin(rot_speed) + obj->env.vec.plane.y * cos(rot_speed);
}

void 	run_mouse(t_mlx *obj)
{
	if (obj->keys.m_left == 1)
		rot_left(obj, obj->m_off);
	if (obj->keys.m_right == 1)
		rot_right(obj, obj->m_off);
}

void	run_keys(t_mlx *obj)
{
	obj->env.sprint = 0.0;
	if (obj->keys.shift == 1)
		obj->env.sprint = MOVE_BUFF;
	if (obj->keys.a == 1)
		move_left(obj);
	if (obj->keys.s == 1)
		move_down(obj);
	if (obj->keys.d == 1)
		move_right(obj);
	if (obj->keys.w == 1)
		move_up(obj);
}

int		run_img(t_mlx *obj)
{
	obj->env.cur_time = get_time();
	run_keys(obj);
	run_mouse(obj);
	init_rays(obj);
	// handle_sprites(obj);
	mlx_put_image_to_window(obj->mlx, obj->win, obj->img, 0, 0);
	get_fps(obj);
	draw_crosshair(obj);
	return (0);
}

int		my_mouse_movement(int x, int y, t_mlx *obj)
{
	obj->keys.m_left = 0;
	obj->keys.m_right = 0;
	if (x > X_ORIGIN + ROT_BUFF && (x <= W_WIDTH && y >= 0 && y <= W_HEIGHT))
	{
		obj->m_off = fabs((double)((x - (X_ORIGIN + ROT_BUFF)) / 100) / 3.5);
		obj->keys.m_right = 1;
		obj->keys.m_left = 0;
	}
	else if (x < X_ORIGIN - ROT_BUFF && (x >= 0 && y >= 0 && y <= W_HEIGHT))
	{
		obj->m_off = fabs((double)((x - (X_ORIGIN - ROT_BUFF)) / 100) / 3.5);
		obj->keys.m_left = 1;
		obj->keys.m_right = 0;
	}
	return (0);
}

int		my_key_press(int keycode, t_mlx *obj)
{
	if (keycode == 53)
		exit_hook(obj);
	if (keycode == 257)
		obj->keys.shift = 1;
	if (keycode == 0)
		obj->keys.a = 1;
	if (keycode == 1)
		obj->keys.s = 1;
	if(keycode == 2)
		obj->keys.d = 1;
	if (keycode == 13)
		obj->keys.w = 1;
	return (0);
}

int		my_key_release(int keycode, t_mlx *obj)
{
	if (keycode == 257)
		obj->keys.shift = 0;
	if (keycode == 0)
		obj->keys.a = 0;
	if (keycode == 1)
		obj->keys.s = 0;
	if(keycode == 2)
		obj->keys.d = 0;
	if (keycode == 13)
		obj->keys.w = 0;
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
	// mlx_mouse_hook(obj->win, my_mouse_func, obj);
	mlx_do_key_autorepeatoff(obj->mlx);
	mlx_hook(obj->win, 6, 0, my_mouse_movement, obj);
	mlx_hook(obj->win, 2, 0, my_key_press, obj);
	mlx_hook(obj->win, 3, 0, my_key_release, obj);
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
	if (!get_texture(obj))
		return (0);
	run_win(obj);
	free(obj);
	return(0);
}
