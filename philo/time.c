/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joandre- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 17:19:11 by joandre-          #+#    #+#             */
/*   Updated: 2024/10/04 02:30:50 by joandre-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

struct timeval	get_time(t_table *tab)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL))
		errmsg("gettimeofday", tab);
	return (time);
}

struct timeval	get_last_meal(t_philo *p)
{
	struct timeval	time;

	if (pthread_mutex_lock(&p->diner))
		errmsg("pthread_mutex_lock", p->tab);
	time = p->last_meal;
	if (pthread_mutex_unlock(&p->diner))
		errmsg("ptherad_mutex_unlock", p->tab);
	return (time);
}

time_t	get_diff(t_philo *p)
{
	struct timeval	clock;
	time_t			ms;

	if (pthread_mutex_lock(&p->diner))
		errmsg("pthread_mutex_lock", p->tab);
	clock = get_time(p->tab);
	ms = ((clock.tv_sec - p->last_meal.tv_sec) * 1000)
		+ ((clock.tv_usec - p->last_meal.tv_usec) / 1000);
	if (pthread_mutex_unlock(&p->diner))
		errmsg("pthread_mutex_unlock", p->tab);
	return (ms);
}
