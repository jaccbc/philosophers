/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joandre- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 17:19:11 by joandre-          #+#    #+#             */
/*   Updated: 2024/09/21 19:00:03 by joandre-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

struct timeval	get_time(void)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL))
		errmsg("gettimeofday");
	return (time);
}

struct timeval	get_last_meal(t_philo *p)
{
	struct timeval	time;

	if (pthread_mutex_lock(&p->diner))
		errmsg("pthread_mutex_lock");
	time = p->last_meal;
	if (pthread_mutex_unlock(&p->diner))
		errmsg("ptherad_mutex_unlock");
	return (time);
}

time_t	get_diff(t_philo *p)
{
	struct timeval	clock;
	time_t			ms;

	if (pthread_mutex_lock(&p->diner))
		errmsg("pthread_mutex_lock");
	clock = get_time();
	ms = ((clock.tv_sec - p->last_meal.tv_sec) * 1000)
		+ ((clock.tv_usec - p->last_meal.tv_usec) / 1000);
	if (pthread_mutex_unlock(&p->diner))
		errmsg("pthread_mutex_unlock");
	return (ms);
}
