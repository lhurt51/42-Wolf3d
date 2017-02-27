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

int		loading_press(int keycode, t_env *obj)
{
	if (keycode == 53)
		exit_hook(obj);
	if (keycode && obj->load_per >= 102)
		run_game(obj);
	return (0);
}

void	run_loader(t_env *obj)
{
	obj->mlx.mlx = mlx_init();
	obj->mlx.win = mlx_new_window(obj->mlx.mlx, W_WIDTH, W_HEIGHT, "Wolf3D");
	if (obj->mlx.img)
		mlx_destroy_image(obj->mlx.mlx, obj->mlx.img);
	obj->mlx.img = mlx_new_image(obj->mlx.mlx, W_WIDTH, W_HEIGHT);
	obj->mlx.data = mlx_get_data_addr(obj->mlx.img, &obj->mlx.bits,
		&obj->mlx.size_line, &obj->mlx.endian);
	mlx_hook(obj->mlx.win, 17, 0, exit_hook, obj);
	mlx_hook(obj->mlx.win, 2, 0, loading_press, obj);
	mlx_loop_hook(obj->mlx.mlx, start_loading, obj);
	mlx_loop(obj->mlx.mlx);
}

void	*load_in(void *in)
{
	t_env	*obj;
	int		size;

	obj = (t_env*)in;
	size = count_ord(obj->m_env.av);
	obj->rtn = 1;
	obj->rtn = (!get_texture(obj) || !read_map(obj, size, 0)) ? 0 : 1;
	obj->load_per = 101;
	sleep(1);
	obj->load_per = 102;
	pthread_exit(&obj->rtn);
	return (NULL);
}

int		main(void)
{
	t_env		*obj;
	int			err;

	obj = malloc(sizeof(t_env));
	if (!obj)
		return ((int)error("Malloc failed"));
	reset_struct(obj);
	obj->m_env.av = ft_strdup("test_map");
	err = pthread_create(&obj->pth, NULL, &load_in, obj);
	if (err != 0)
		return ((int)error("Creating a thread failed"));
	run_loader(obj);
	exit_hook(obj);
	return (0);
}
