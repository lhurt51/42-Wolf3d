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
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,2,2,2,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
	{1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1},
	{1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,4,0,0,0,0,5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

int		exit_hook(t_mlx *obj)
{
	free(obj);
	exit(0);
}

long	get_time(void)
{
	clock_serv_t	cclock;
	mach_timespec_t	mts;

	host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
	clock_get_time(cclock, &mts);
	mach_port_deallocate(mach_task_self(), cclock);
	// t->tv_sec = mts.tv_sec;
	// t->tv_nsec = mts.tv_nsec;
	return (mts.tv_nsec);
}

void	set_env(t_mlx *obj, t_intpoint *map, int x)
{
	//calculate ray position and direction
	obj->env.ray_dir.x = obj->env.dir.x + obj->env.plane.x * CAMERA_X(x);
	obj->env.ray_dir.y = obj->env.dir.y + obj->env.plane.y * CAMERA_X(x);
	//which box of the map we're in
	map->x = (int)obj->env.pos.x;
	map->y = (int)obj->env.pos.y;
	//length of ray from one x or y-side to next x or y-side
	obj->env.delta_dist.x = DELTADIST(obj->env.ray_dir.x, obj->env.ray_dir.y);
	obj->env.delta_dist.y = DELTADIST(obj->env.ray_dir.y, obj->env.ray_dir.x);
}

void	find_dir_step(t_mlx *obj, t_intpoint *map, t_point *side_dist)
{
	//calculate step and initial sideDist
	if (obj->env.ray_dir.x < 0)
	{
		obj->env.step.x = -1;
		side_dist->x = (obj->env.pos.x - map->x) * obj->env.delta_dist.x;
	}
	else
	{
		obj->env.step.x = 1;
		side_dist->x = (map->x + 1.0 - obj->env.pos.x) * obj->env.delta_dist.x;
	}
	if (obj->env.ray_dir.y < 0)
	{
		obj->env.step.y = -1;
		side_dist->y = (obj->env.pos.y - map->y) * obj->env.delta_dist.y;
	}
	else
	{
		obj->env.step.y = 1;
		side_dist->y = (map->y + 1.0 - obj->env.pos.y) * obj->env.delta_dist.y;
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
			 side_dist->x += obj->env.delta_dist.x;
			 map->x += obj->env.step.x;
			 obj->env.side = 0;
		}
		else
		{
			 side_dist->y += obj->env.delta_dist.y;
			 map->y += obj->env.step.y;
			 obj->env.side = 1;
		}
		//Check if ray has hit a wall
		if (worldmap[map->x][map->y] > 0)
			hit = 1;
	}
}

void	find_draw_pnts(t_mlx *obj, t_intpoint *map)
{
	int		line_h;
	double	wall_dist;

	//Calculate height of line to draw on screen
	if (obj->env.side == 0)
		wall_dist = (map->x - obj->env.pos.x + (1 - obj->env.step.x) / 2) / obj->env.ray_dir.x;
	else
		wall_dist = (map->y - obj->env.pos.y + (1 - obj->env.step.y) / 2) / obj->env.ray_dir.y;
	line_h = (int)(W_HEIGHT * 3 / wall_dist);
	//calculate lowest and highest pixel to fill in current stripe
	obj->env.draw_start = -line_h / 2 + W_HEIGHT / 2;
	if(obj->env.draw_start < 0)
		obj->env.draw_start = 0;
	obj->env.draw_end = line_h / 2 + W_HEIGHT / 2;
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

void	init_rays(t_mlx *obj)
{
	int 		x;
	// struct 		timespec t;
	t_point		side_dist;
	t_intpoint	map;
	t_intpoint	p1;
	t_intpoint	p2;

	x = W_WIDTH;
	while (x > 0)
	{
		set_env(obj, &map, x);
		find_dir_step(obj, &map, &side_dist);
		exe_dda(obj, &map, &side_dist);
		find_draw_pnts(obj, &map);
		choose_color(obj, &map);
		p1.x = x;
		p2.x = x;
		p1.y = obj->env.draw_start;
		p2.y = obj->env.draw_end;
		get_da(obj, &p1, &p2);
		x--;
	}
}

void	reset_struct(t_mlx *obj)
{
	obj->env.pos.x = 22;
	obj->env.pos.y = 12;
	obj->env.dir.x = -1;
	obj->env.dir.y = 0.0;
	obj->env.plane.x = 0.0;
	obj->env.plane.y = 0.66;
	obj->env.cur_time = 0.0;
}

void	run_img(t_mlx *obj)
{
	mlx_destroy_image(obj->mlx, obj->img);
	obj->img = mlx_new_image(obj->mlx, W_WIDTH, W_HEIGHT);
	obj->data = mlx_get_data_addr(obj->img, &obj->bits, &obj->size_line,
		&obj->endian);
	obj->env.cur_time = get_time();
	init_rays(obj);
	mlx_put_image_to_window(obj->mlx, obj->win, obj->img, 0, 0);
	get_fps(obj);
}

void	move_up(t_mlx *obj)
{
	double	move_speed;

	move_speed = obj->env.frame_time * 15.0;
	if(worldmap[(int)(obj->env.pos.x + obj->env.dir.x * move_speed)][(int)obj->env.pos.y] == 0)
		obj->env.pos.x += obj->env.dir.x * move_speed;
	if(worldmap[(int)obj->env.pos.x][(int)(obj->env.pos.y + obj->env.dir.y * move_speed)] == 0)
		obj->env.pos.y += obj->env.dir.y * move_speed;
	run_img(obj);
}

void	move_down(t_mlx *obj)
{
	double	move_speed;

	move_speed = obj->env.frame_time * 15.0;
	if(worldmap[(int)(obj->env.pos.x - obj->env.dir.x * move_speed)][(int)obj->env.pos.y] == 0)
		obj->env.pos.x -= obj->env.dir.x * move_speed;
	if(worldmap[(int)obj->env.pos.x][(int)(obj->env.pos.y - obj->env.dir.y * move_speed)] == 0)
		obj->env.pos.y -= obj->env.dir.y * move_speed;
	run_img(obj);
}

void	rot_right(t_mlx *obj)
{
	double	old_dirx;
	double	old_planex;
	double	rot_speed;

	rot_speed = obj->env.frame_time * 3.0;
	// rot_speed = (M_PI / 18);
	old_dirx = obj->env.dir.x;
	old_planex = obj->env.plane.x;
	obj->env.dir.x = obj->env.dir.x * cos(-rot_speed) - obj->env.dir.y * sin(-rot_speed);
	obj->env.dir.y = old_dirx * sin(-rot_speed) + obj->env.dir.y * cos(-rot_speed);
	obj->env.plane.x = obj->env.plane.x * cos(-rot_speed) - obj->env.plane.y * sin(-rot_speed);
	obj->env.plane.y = old_planex * sin(-rot_speed) + obj->env.plane.y * cos(-rot_speed);
	run_img(obj);
}

void	rot_left(t_mlx *obj)
{
	double	old_dirx;
	double	old_planex;
	double	rot_speed;

	rot_speed = obj->env.frame_time * 3.0;
	// rot_speed = (M_PI / 18);
	old_dirx = obj->env.dir.x;
	old_planex = obj->env.plane.x;
	obj->env.dir.x = obj->env.dir.x * cos(rot_speed) - obj->env.dir.y * sin(rot_speed);
	obj->env.dir.y = old_dirx * sin(rot_speed) + obj->env.dir.y * cos(rot_speed);
	obj->env.plane.x = obj->env.plane.x * cos(rot_speed) - obj->env.plane.y * sin(rot_speed);
	obj->env.plane.y = old_planex * sin(rot_speed) + obj->env.plane.y * cos(rot_speed);
	run_img(obj);
}

int		my_key_press(int keycode, t_mlx *obj)
{
	printf("Keycode: %d\n", keycode);
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
	obj->img = mlx_new_image(obj->mlx, W_WIDTH, W_HEIGHT);
	reset_struct(obj);
	run_img(obj);
	// mlx_mouse_hook(obj->win, my_mouse_func, obj);
	// mlx_hook(obj->win, 6, 0, my_mouse_movement, obj);
	mlx_hook(obj->win, 2, 0, my_key_press, obj);
	mlx_hook(obj->win, 17, 0, exit_hook, obj);
	mlx_loop(obj->mlx);
}

int		main()
{
	t_mlx	*obj;

	obj = malloc(sizeof(t_mlx));
	if(!obj)
		return ((int)error("Malloc failed"));
	run_win(obj);
	return(0);
}
