/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lhurt <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/26 22:40:41 by lhurt             #+#    #+#             */
/*   Updated: 2017/02/26 22:40:43 by lhurt            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf.h"

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
