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

int		main()
{
	t_mlx	*new;
	double pos_x; // x & y starting pos
	double pos_y;
	double dir_x; // starting direction vectors
	double dir_y;
	double plane_x;
	double plane_y;
	double cur_time;
	double old_time;

	pos_x = 22;
	pos_y = 12;
	dir_x = -1;
	dir_y = 0;
	plane_x = 0;
	plane_y = 0.66;
	cur_time = 0;
	old_time = 0;
	new = malloc(sizeof(t_mlx));
	if(!new)
		return (0);
	new->mlx = mlx_init();
	new->win = mlx_new_window(new->mlx, W_WIDTH, W_HEIGHT, "Wolf3D");
	new->img = mlx_new_image(new->mlx, W_WIDTH, W_HEIGHT);
	new->data = mlx_get_data_addr(new->img, &new->bits, &new->size_line,
		&new->endian);
	int x;
	x = 0;
	while (x < W_WIDTH)
	{
		//calculate ray position and direction
		double camera_x = 2 * x / (double)(W_WIDTH) - 1; //x-coordinate in camera space
		double ray_pos_x = pos_x;
		double ray_pos_y = pos_y;
		double ray_dir_x = dir_x + plane_x * camera_x;
		double ray_dir_y = dir_y + plane_y * camera_x;
		//which box of the map we're in
		int map_x = (int)ray_pos_x;
		int map_y = (int)ray_pos_y;
		//length of ray from current position to next x or y-side
		double sideDistX;
		double sideDistY;
		//length of ray from one x or y-side to next x or y-side
		double deltaDistX = sqrt(1 + (ray_dir_y * ray_dir_y) / (ray_dir_x * ray_dir_x));
		double deltaDistY = sqrt(1 + (ray_dir_x * ray_dir_x) / (ray_dir_y * ray_dir_y));
		double perpWallDist;
		//what direction to step in x or y-direction (either +1 or -1)
		int stepX;
		int stepY;
		int hit = 0; //was there a wall hit?
		int side; //was a NS or a EW wall hit?
		//calculate step and initial sideDist
		if (ray_dir_x < 0)
		{
			stepX = -1;
			sideDistX = (ray_pos_x - map_x) * deltaDistX;
		}
		else
		{
			stepX = 1;
			sideDistX = (map_x + 1.0 - ray_pos_x) * deltaDistX;
		}
		if (ray_dir_y < 0)
		{
			stepY = -1;
			sideDistY = (ray_pos_y - map_y) * deltaDistY;
		}
		else
		{
			stepY = 1;
			sideDistY = (map_y + 1.0 - ray_pos_y) * deltaDistY;
		}
		//perform DDA
		while (hit == 0)
		{
			//jump to next map square, OR in x-direction, OR in y-direction
			if (sideDistX < sideDistY)
			{
				 sideDistX += deltaDistX;
				 map_x += stepX;
				 side = 0;
			}
			else
			{
				 sideDistY += deltaDistY;
				 map_y += stepY;
				 side = 1;
			}
			//Check if ray has hit a wall
			if (worldmap[map_x][map_y] > 0)
				hit = 1;
		}
		if (side == 0)
			perpWallDist = (map_x - ray_pos_x + (1 - stepX) / 2) / ray_dir_x;
  		else
			perpWallDist = (map_y - ray_pos_y + (1 - stepY) / 2) / ray_dir_y;
		//Calculate height of line to draw on screen
		int lineHeight = (int)(W_HEIGHT / perpWallDist);
		//calculate lowest and highest pixel to fill in current stripe
		int drawStart = -lineHeight / 2 + W_HEIGHT / 2;
		if(drawStart < 0)
			drawStart = 0;
		int drawEnd = lineHeight / 2 + W_HEIGHT / 2;
		if(drawEnd >= W_HEIGHT)
			drawEnd = W_HEIGHT - 1;
		//choose wall color
		int color;
		switch (worldmap[map_x][map_y])
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
		if (side == 1)
			color = color / 2;
		//draw the pixels of the stripe as a vertical line
		new->env.color = color;
		t_point		p1, p2;
		p1.x = x;
		p2.x = x;
		p1.y = drawStart;
		p2.y = drawEnd;
		printf("Point1  X: %f, Y: %f\n", p1.x, p1.y);
		printf("Point2  X: %f, Y: %f\n", p2.x, p2.y);
		get_da(new, &p1, &p2);
		x++;
	}

	mlx_put_image_to_window(new->mlx, new->win, new->img, 0, 0);
	// mlx_mouse_hook(new->win, my_mouse_func, new);
	// mlx_hook(new->win, 6, 0, my_mouse_movement, new);
	// mlx_hook(new->win, 2, 0, my_key_press, new);
	// mlx_hook(new->window, 17, 0, exit_hook, new);
	mlx_loop(new->mlx);
}
