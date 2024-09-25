/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joandre- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/21 01:03:54 by joandre-          #+#    #+#             */
/*   Updated: 2024/09/25 17:54:45 by joandre-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	all_ate(t_philo *p)
{
	bool	result;

	pthread_mutex_lock(&p->diner);
	if (p->tab->must_eat == p->meal && p->tab->must_eat)
		result = true;
	else
		result = false;
	pthread_mutex_unlock(&p->diner);
	return (result);
}

static void	monitor_dead(t_table *tab)
{
	unsigned int	i;
	struct timeval	last_meal;
	struct timeval	clock;
	time_t			diff;

	i = 0;
	while (i < tab->p_count)
	{
		if (!all_ate(&tab->p[i++]))
		{
			last_meal = get_last_meal(&tab->p[i - 1]);
			clock = get_time();
			diff = ((clock.tv_sec - last_meal.tv_sec) * 1000)
				+ ((clock.tv_usec - last_meal.tv_usec) / 1000);
			if (tab->time_to_die + 1 <= diff)
			{
				print_log("died", &tab->p[--i], clock);
				set_bool(&tab->starv, &tab->starvation, true);
				break ;
			}
		}
	}
}

void	*monitor(void *data)
{
	unsigned int	i;
	t_table			*tab;

	tab = (t_table *)data;
	while (!get_bool(&tab->main, &tab->start))
		continue ;
	usleep((tab->time_to_die * 1000) / 2);
	while (!get_bool(&tab->starv, &tab->starvation))
	{
		monitor_dead(tab);
		i = 0;
		while ((i < tab->p_count) && all_ate(&tab->p[i]))
			if (++i == tab->p_count)
				return (NULL);
	}
	return (NULL);
}
