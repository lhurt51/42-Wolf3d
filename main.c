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

void	set_env(t_env *obj, t_point *map, int x)
{
	//calculate ray position and direction
	obj->vec.ray_dir.x = obj->vec.dir.x + obj->vec.plane.x * CAMERA_X(x);
	obj->vec.ray_dir.y = obj->vec.dir.y + obj->vec.plane.y * CAMERA_X(x);
	//which box of the map we're in
	map->x = (int)obj->vec.pos.x;
	map->y = (int)obj->vec.pos.y;
	//length of ray from one x or y-side to next x or y-side
	obj->vec.delta_dist.x = DELTADIST(obj->vec.ray_dir.x, obj->vec.ray_dir.y);
	obj->vec.delta_dist.y = DELTADIST(obj->vec.ray_dir.y, obj->vec.ray_dir.x);
}

void	find_dir_step(t_env *obj, t_point *map, t_point *side_dist)
{
	//calculate step and initial sideDist
	if (obj->vec.ray_dir.x < 0)
	{
		obj->vec.step.x = -1;
		side_dist->x = (obj->vec.pos.x - map->x) * obj->vec.delta_dist.x;
	}
	else
	{
		obj->vec.step.x = 1;
		side_dist->x = (map->x + 1.0 - obj->vec.pos.x) * obj->vec.delta_dist.x;
	}
	if (obj->vec.ray_dir.y < 0)
	{
		obj->vec.step.y = -1;
		side_dist->y = (obj->vec.pos.y - map->y) * obj->vec.delta_dist.y;
	}
	else
	{
		obj->vec.step.y = 1;
		side_dist->y = (map->y + 1.0 - obj->vec.pos.y) * obj->vec.delta_dist.y;
	}
}

void	exe_dda(t_env *obj, t_point *map, t_point *side_dist)
{
	int hit; //was there a wall hit?

	hit = 0;
	while (hit == 0)
	{
		//jump to next map square, OR in x-direction, OR in y-direction
		if (side_dist->x < side_dist->y)
		{
			 side_dist->x += obj->vec.delta_dist.x;
			 map->x += obj->vec.step.x;
			 obj->var.side = 0;
		}
		else
		{
			 side_dist->y += obj->vec.delta_dist.y;
			 map->y += obj->vec.step.y;
			 obj->var.side = 1;
		}
		//Check if ray has hit a wall
		if (obj->m_env.map[(int)map->x][(int)map->y] > 0)
			hit = 1;
	}
}

void	find_draw_pnts(t_env *obj, t_point *map)
{
	//Calculate height of line to draw on screen
	if (obj->var.side == 0)
		obj->var.wall_dist = (map->x - obj->vec.pos.x + (1 - obj->vec.step.x) / 2) / obj->vec.ray_dir.x;
	else
		obj->var.wall_dist = (map->y - obj->vec.pos.y + (1 - obj->vec.step.y) / 2) / obj->vec.ray_dir.y;
	obj->var.line_h = (int)(W_HEIGHT / obj->var.wall_dist);
	//calculate lowest and highest pixel to fill in current stripe
	obj->var.draw_start = -obj->var.line_h / 2 + Y_ORIGIN;
	if(obj->var.draw_start < 0)
		obj->var.draw_start = 0;
	obj->var.draw_end = obj->var.line_h / 2 + Y_ORIGIN;
	if(obj->var.draw_end  >= W_HEIGHT)
		obj->var.draw_end  = W_HEIGHT - 1;
}

void	get_fps(t_env *obj)
{
	double	old_time;
	char	*print;
	char	*tmp;

	old_time = obj->var.cur_time;
	obj->var.cur_time = get_time();
	if (obj->var.cur_time < old_time)
		obj->var.cur_time += 1000000000;
	obj->var.frame_time = (double)(obj->var.cur_time - old_time) / 1000000000.0;
	print = ft_strnew(5);
	print = "FPS: ";
	tmp = ft_itoa((int)1.0 / obj->var.frame_time);
	print = ft_strjoin(print, tmp);
	mlx_string_put(obj->mlx.mlx, obj->mlx.win, 0, 0, 0xFFFFFF, print);
	ft_strdel(&tmp);
	ft_strdel(&print);
}

int		set_tex_var(t_env *obj)
{
	int		tex_x;

	if (obj->var.side == 0)
		obj->var.wall_x = obj->vec.pos.y + obj->var.wall_dist * obj->vec.ray_dir.y;
	else
		obj->var.wall_x = obj->vec.pos.x + obj->var.wall_dist * obj->vec.ray_dir.x;
	obj->var.wall_x -= floor(obj->var.wall_x);

	tex_x = (int)(obj->var.wall_x * (double)(T_SIZE));
	if (obj->var.side == 0 && obj->vec.ray_dir.x > 0)
		tex_x = T_SIZE - tex_x - 1;
	if (obj->var.side == 1 && obj->vec.ray_dir.y < 0)
		tex_x = T_SIZE - tex_x - 1;
	return (tex_x);
}

void	choose_tex(t_env *obj, t_point *map, int x)
{
	t_point tex;
	int		ran;
	int		y;
	int 	tex_num;

	tex_num = obj->m_env.map[(int)map->x][(int)map->y] - 1;
	tex_num = obj->tex_pal[tex_num][obj->var.side];
	tex.x = abs(set_tex_var(obj));
	y = (int)obj->var.draw_start;
	while (y < (int)obj->var.draw_end)
	{
		ran = y * 256 - W_HEIGHT * 128 + obj->var.line_h * 128;
		tex.y = abs(((ran * T_SIZE) / obj->var.line_h) / 256);
		if ((x < W_WIDTH && x >= 0) && (y < W_HEIGHT && y >= 0))
			pixel_to_img(obj, x, y, obj->tex[tex_num][(int)tex.y][(int)tex.x]);
		y++;
	}
}

void	draw_floor(t_env *obj, t_point *floor_wall, int x)
{
	t_point		cur_floor;
	t_point		floor_tex;
	double		cur_dist;
	double		weight;
	int			y;

	if (obj->var.draw_end < 0) obj->var.draw_end = W_HEIGHT;
	y = obj->var.draw_end + 1;
	while (y <= W_HEIGHT)
	{
		cur_dist = W_HEIGHT / (2.0 * y - W_HEIGHT);
		weight = (cur_dist - 0.0) / (obj->var.wall_dist - 0.0);
		cur_floor.x = weight * floor_wall->x + (1.0 - weight) * obj->vec.pos.x;
		cur_floor.y = weight * floor_wall->y + (1.0 - weight) * obj->vec.pos.y;
		floor_tex.x = abs((int)(cur_floor.x * T_SIZE) % T_SIZE);
		floor_tex.y = abs((int)(cur_floor.y * T_SIZE) % T_SIZE);
		if ((x < W_WIDTH && x >= 0) && (y < W_HEIGHT && y >= 0))
			pixel_to_img(obj, x, y,
				obj->tex[0][(int)floor_tex.y][(int)floor_tex.x]);
		if ((x < W_WIDTH && x >= 0) && (W_HEIGHT - y < W_HEIGHT &&
				W_HEIGHT - y >= 0))
			pixel_to_img(obj, x, W_HEIGHT - y,
				obj->tex[17][(int)floor_tex.y][(int)floor_tex.x]);
		y++;
	}
}


void	floor_casting(t_env *obj, t_point *map, int x)
{
	t_point		floor_wall;

	if (obj->var.side == 0 && obj->vec.ray_dir.x > 0)
	{
		floor_wall.x = map->x;
		floor_wall.y = map->y + obj->var.wall_x;
	}
	else if (obj->var.side == 0 && obj->vec.ray_dir.x < 0)
	{
		floor_wall.x = map->x + 1.0;
		floor_wall.y = map->y + obj->var.wall_x;
	}
	else if (obj->var.side == 1 && obj->vec.ray_dir.y > 0)
	{
		floor_wall.x = map->x + obj->var.wall_x;
		floor_wall.y = map->y;
	}
	else
	{
		floor_wall.x = map->x + obj->var.wall_x;
		floor_wall.y = map->y + 1.0;
	}
	draw_floor(obj, &floor_wall, x);
}

void	init_rays(t_env *obj)
{
	int 		x;
	t_point		side_dist;
	t_point		map;

	x = W_WIDTH + 1;
	while (x-- > 0)
	{
		set_env(obj, &map, x);
		find_dir_step(obj, &map, &side_dist);
		exe_dda(obj, &map, &side_dist);
		find_draw_pnts(obj, &map);
		choose_tex(obj, &map, x);
		obj->z_buff[x] = obj->var.wall_dist;
		floor_casting(obj, &map, x);
	}
}

void	reset_struct(t_env *obj)
{
	obj->vec.pos.x = 11.0;
	obj->vec.pos.y = 11.0;
	obj->vec.dir.x = -1.0;
	obj->vec.dir.y = 0.0;
	obj->vec.plane.x = 0.0;
	obj->vec.plane.y = 0.66;
	obj->var.wall_x = 0;
	obj->load_per = 0;
	obj->mlx.keys.m_left = 0;
	obj->mlx.keys.m_right = 0;
	obj->mlx.keys.shift = 0;
	obj->mlx.keys.a = 0;
	obj->mlx.keys.s = 0;
	obj->mlx.keys.d = 0;
	obj->mlx.keys.w = 0;
}

void 		draw_chline(t_env *obj, int prim, int secn, int check)
{
	int x;
	int y;

	x = prim + CH_LEN + 1;
	while (x-- > prim - CH_LEN)
	{
		y = secn + CH_WID + 1;
		while (y-- > secn - CH_WID)
		{
			if (x > prim + CH_OFF || x < prim - CH_OFF)
			{
				if (check)
					mlx_pixel_put(obj->mlx.mlx, obj->mlx.win, x, y, CH_COLOR);
				else
					mlx_pixel_put(obj->mlx.mlx, obj->mlx.win, y, x, CH_COLOR);
			}
		}
	}
}

void		draw_crosshair(t_env *obj)
{
	draw_chline(obj, X_ORIGIN, Y_ORIGIN, 1);
	draw_chline(obj, Y_ORIGIN, X_ORIGIN, 0);
}

void	draw_sprites(t_env *obj, t_sort *ord)
{
	t_point	sprite;
	t_point	trans;
	t_point start;
	t_point	end;
	t_point tex;
	double	mat;
	int		w_sprite_x;
	int		sprite_h;
	int		stripe;
	int		color;
	int		ans;
	int		tmp;
	int		y;
	int		i;

	i = obj->m_env.num_s;
	while (i-- > 0)
	{
		tmp = ord[i].index;
		sprite.x = obj->m_env.sprites[tmp].pnt.x - obj->vec.pos.x;
		sprite.y = obj->m_env.sprites[tmp].pnt.y - obj->vec.pos.y;
		mat = 1.0 / (obj->vec.plane.x * obj->vec.dir.y - obj->vec.dir.x * obj->vec.plane.y);
		trans.x = mat * (obj->vec.dir.y * sprite.x - obj->vec.dir.x * sprite.y);
		trans.y = mat * (-obj->vec.plane.y * sprite.x + obj->vec.plane.x * sprite.y);
		w_sprite_x = (int)((X_ORIGIN * (1 + trans.x / trans.y)));
		sprite_h = abs((int)(W_HEIGHT / trans.y));
		start.x = -sprite_h / 2 + w_sprite_x;
		if (start.x < 0)
			start.x = 0;
		start.y = -sprite_h / 2 + Y_ORIGIN;
		if (start.y < 0)
			start.y = 0;
		end.x = sprite_h / 2 + w_sprite_x;
		if (end.x >= W_WIDTH)
			end.x = W_WIDTH - 1;
		end.y = sprite_h / 2 + Y_ORIGIN;
		if (end.y >= W_HEIGHT)
			end.y = W_HEIGHT - 1;
		stripe = start.x;
		while (stripe < end.x)
		{
			tex.x = abs((int)(256 * (stripe - (-sprite_h / 2 + w_sprite_x)) * T_SIZE / sprite_h) / 256);
			if (trans.y > 0 && stripe > 0 && stripe < W_WIDTH && trans.y < obj->z_buff[stripe])
			{
				y = start.y;
				while (y < end.y)
				{
					tmp = ord[i].index;
					tmp = obj->m_env.sprites[tmp].tex;
					ans = y * 256 - W_HEIGHT * 128 + sprite_h * 128;
					tex.y = abs((int)((ans * T_SIZE) / sprite_h) / 256);
					color = obj->tex[tmp][(int)tex.y][(int)tex.x];
					if ((color != 0x980088 && color != 0x9B038B && color >= 0) && (stripe < W_WIDTH && stripe >= 0) && (y < W_HEIGHT && y >= 0))
						pixel_to_img(obj, stripe, y, color);
					y++;
				}
			}
			stripe++;
		}
	}
}

void	swap_allmem(t_sort *a, t_sort *b)
{
	swap_memin(&a->index, &b->index);
	swap_mem(&a->dist, &b->dist);
}

void	ft_qsort(t_sort *a, int len, int start)
{
	int	piv;
	int	i;
	int	j;

	if (start < len)
	{
		piv = start;
		i = start;
		j = len;
		while (i < j)
		{
			while (a[i].dist <= a[piv].dist && i <= len)
				i++;
			while (a[j].dist > a[piv].dist)
				j--;
			if (i < j)
				swap_allmem(&a[i], &a[j]);
		}
		swap_allmem(&a[piv], &a[j]);
		ft_qsort(a, j - 1, start);
		ft_qsort(a, len, j + 1);
	}
}

void	handle_sprites(t_env *obj)
{
	t_sort	*sort;
	int		i;

	i = obj->m_env.num_s;
	sort = malloc(sizeof(t_sort) * obj->m_env.num_s);
	if (!sort)
		exit_hook(obj);
	while (i-- > 0)
	{
		sort[i].index = i;
		sort[i].dist = ((obj->vec.pos.x - obj->m_env.sprites[i].pnt.x) * (obj->vec.pos.x  - obj->m_env.sprites[i].pnt.x) + (obj->vec.pos.y - obj->m_env.sprites[i].pnt.y) * (obj->vec.pos.y - obj->m_env.sprites[i].pnt.y));
	}
	ft_qsort(sort, obj->m_env.num_s - 1, 0);
	draw_sprites(obj, sort);
	ft_memdel((void**)&sort);
}

int		run_img(t_env *obj)
{
	run_keys(obj);
	run_mouse(obj);
	obj->var.cur_time = get_time();
	init_rays(obj);
	handle_sprites(obj);
	mlx_put_image_to_window(obj->mlx.mlx, obj->mlx.win, obj->mlx.img, 0, 0);
	get_fps(obj);
	draw_crosshair(obj);
	return (0);
}

void	print_board(int **board, int w, int l)
{
	int j;
	int x;

	j = 0;
	while (j < w)
	{
		x = 0;
		while (x < l)
		{
			ft_putnbr(board[j][x]);
			ft_putchar(' ');
			x++;
		}
		ft_putchar('\n');
		j++;
	}
}

void 	run_game(t_env *obj)
{
	if (obj->mlx.win)
		mlx_clear_window(obj->mlx.mlx, obj->mlx.win);
	else
		obj->mlx.win = mlx_new_window(obj->mlx.mlx, W_WIDTH, W_HEIGHT, "Wolf3D");
	if (obj->mlx.img)
		mlx_destroy_image(obj->mlx.mlx, obj->mlx.img);
	obj->mlx.img = mlx_new_image(obj->mlx.mlx, W_WIDTH, W_HEIGHT);
	obj->mlx.data = mlx_get_data_addr(obj->mlx.img, &obj->mlx.bits, &obj->mlx.size_line,
		&obj->mlx.endian);
	// mlx_mouse_hook(obj->win, my_mouse_func, obj);
	mlx_do_key_autorepeatoff(obj->mlx.mlx);
	mlx_hook(obj->mlx.win, 6, 0, my_mouse_movement, obj);
	mlx_hook(obj->mlx.win, 2, 0, my_key_press, obj);
	mlx_hook(obj->mlx.win, 3, 0, my_key_release, obj);
	mlx_hook(obj->mlx.win, 17, 0, exit_hook, obj);
	mlx_loop_hook(obj->mlx.mlx, run_img, obj);
	mlx_loop(obj->mlx.mlx);
}

int		loading_press(int keycode, t_env *obj)
{
	if (keycode == 53)
		exit_hook(obj);
	if (keycode && obj->load_per >= 101)
		run_game(obj);
	return (0);
}

int		start_loading(t_env *obj)
{
	int	size;
	int x;
	int	y;

	if (obj->load_per > 120)
	{
		obj->mlx.img = mlx_xpm_file_to_image(obj->mlx.mlx, "XMP_textures/wolfenstein-the-new-order-logo-wallpaper.XPM", &size, &size);
		mlx_put_image_to_window(obj->mlx.mlx, obj->mlx.win, obj->mlx.img, 0, 0);
		mlx_string_put(obj->mlx.mlx, obj->mlx.win, 5, 5, 0xFFFFFF, "Game Build by Liam Hurt, Only Intended for Educational Purposes");
		mlx_string_put(obj->mlx.mlx, obj->mlx.win, X_ORIGIN - 130, Y_ORIGIN + 275, 0xFFFFFF, "-- Press any key to continue! --");
	}
	else
	{
		x = 29;
		while (x++ < (int)(W_WIDTH * (obj->load_per * 0.01)) && x <= W_WIDTH - 30)
		{
			y = Y_ORIGIN - 21;
			while (y++ < Y_ORIGIN + 20)
				pixel_to_img(obj, x, y, 0xFFFFFF);
		}
		mlx_put_image_to_window(obj->mlx.mlx, obj->mlx.win, obj->mlx.img, 0, 0);
		usleep(60000);
		if (obj->load_per > 100)
			mlx_string_put(obj->mlx.mlx, obj->mlx.win, X_ORIGIN - 70, Y_ORIGIN + 40, 0xFFFFFF, "Loading Completed!");
		obj->load_per++;
	}
	return (0);
}

void 	run_loader(t_env *obj)
{
	obj->mlx.mlx = mlx_init();
	obj->mlx.win = mlx_new_window(obj->mlx.mlx, W_WIDTH, W_HEIGHT, "Wolf3D");
	if (obj->mlx.img)
		mlx_destroy_image(obj->mlx.mlx, obj->mlx.img);
	obj->mlx.img = mlx_new_image(obj->mlx.mlx, W_WIDTH, W_HEIGHT);
	obj->mlx.data = mlx_get_data_addr(obj->mlx.img, &obj->mlx.bits, &obj->mlx.size_line,
		&obj->mlx.endian);
	mlx_hook(obj->mlx.win, 17, 0, exit_hook, obj);
	mlx_hook(obj->mlx.win, 2, 0, loading_press, obj);
	mlx_loop_hook(obj->mlx.mlx, start_loading, obj);
	mlx_loop(obj->mlx.mlx);
}

int		store_info(t_env *obj, char *str)
{
	char	**tmp;
	int		i;

	tmp = ft_strsplit(str, ' ');
	if (count_lines(tmp) != 3)
		return (0);
	obj->m_env.num_s = ft_atoi(tmp[0]);
	obj->m_env.width = ft_atoi(tmp[1]);
	obj->m_env.height = ft_atoi(tmp[2]);
	obj->m_env.sprites = (t_sprite*)malloc(sizeof(t_sprite) * obj->m_env.num_s);
	if (!obj->m_env.sprites)
		return (0);
	obj->m_env.map = (int**)malloc(sizeof(int*) * obj->m_env.height);
	if (!obj->m_env.map)
		return (0);
	i = 0;
	while (tmp[i])
		ft_strdel(&tmp[i++]);
	return (1);
}

int		store_sprite(t_env *obj, char *str, int i)
{
	char **tmp;
	char *ext;

	tmp = ft_strsplit(str, '"');
	if (count_lines(tmp) != 2)
		return (0);
	obj->m_env.sprites[i].tex = ft_atoi(tmp[1]);
	tmp = ft_strsplit(tmp[0], ' ');
	if (count_lines(tmp) != 2)
		return (0);
	ext = tmp[1];
	tmp = ft_strsplit(tmp[0], '.');
	if (count_lines(tmp) != 2)
		return (0);
	obj->m_env.sprites[i].pnt.x = ft_atoi(tmp[0]) + (double)ft_atoi(tmp[1]) / 10;
	tmp = ft_strsplit(ext, '.');
	if (count_lines(tmp) != 2)
		return (0);
	obj->m_env.sprites[i].pnt.y = ft_atoi(tmp[0]) + (double)ft_atoi(tmp[1]) / 10;
	i = 0;
	while (tmp[i])
		ft_strdel(&tmp[i++]);
	return (1);
}

int		store_map(t_env *obj, char *str, int x)
{
	char	**tmp;
	int		i;

	obj->m_env.map[x] = (int*)malloc(sizeof(int) * obj->m_env.width);
	if (!obj->m_env.map[x])
		return (0);
	tmp = ft_strsplit(str, ' ');
	if ((int)count_lines(tmp) != obj->m_env.width)
		return (0);
	i = obj->m_env.width;
	while (i-- > 0)
		obj->m_env.map[x][i] = ft_atoi(tmp[i]);
	i = 0;
	while (tmp[i])
		ft_strdel(&tmp[i++]);
	return (1);
}

int		read_map(t_env *obj)
{
	char	*tmp;
	int		check;
	int		i;
	int		fd;

	i = 0;
	fd = open(obj->m_env.av, O_RDONLY);
	if (fd < 0)
		return ((int)error(ft_strjoin("Can't open file: ", obj->m_env.av)));
	while (get_next_line(fd, &tmp))
	{
		if (i == 0)
			check = store_info(obj, tmp);
		else if (i > 1 && i < obj->m_env.num_s + 2)
			check = store_sprite(obj, tmp, i - 2);
		else if (i > obj->m_env.num_s + 2 && i < obj->m_env.num_s + obj->m_env.height + 3)
			check = store_map(obj, tmp, (i - obj->m_env.num_s - 3));
		if (check == 0)
			return ((int)error(ft_strjoin(obj->m_env.av, ", has the wrong file format")));
		i++;
	}
	if ((i - obj->m_env.num_s - 3) != obj->m_env.height)
		return ((int)error(ft_strjoin(obj->m_env.av, ", has the wrong file height")));
	return (1);
}

int		main()
{
	t_env		*obj;

	obj = malloc(sizeof(t_env));
	if(!obj)
		return ((int)error("Malloc failed"));
	reset_struct(obj);
	obj->m_env.av = ft_strdup("test_tex");
	if (!get_texture(obj) || !read_map(obj))
		return (0);
	run_loader(obj);
	exit_hook(obj);
	return(0);
}
