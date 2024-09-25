/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   diner.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joandre- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 22:05:12 by joandre-          #+#    #+#             */
/*   Updated: 2024/09/25 17:49:13 by joandre-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static struct timeval	meal_time(t_philo *p)
{
	struct timeval	time;

	if (pthread_mutex_lock(&p->diner))
		errmsg("pthread_mutex_lock");
	p->meal++;
	time = get_time();
	p->last_meal = time;
	if (pthread_mutex_unlock(&p->diner))
		errmsg("pthread_mutex_unlock");
	return (time);
}

static void	time_to_sleep(t_philo *p)
{
	time_t	die;
	time_t	sleep;
	time_t	eat;

	die = p->tab->time_to_die;
	sleep = p->tab->time_to_sleep;
	eat = p->tab->time_to_eat;
	if (!(get_bool(&p->tab->starv, &p->tab->starvation)))
		print_log("is sleeping", p, get_time());
	while (eat + sleep > get_diff(p))
		usleep(get_diff(p));
	if (!(get_bool(&p->tab->starv, &p->tab->starvation)))
		print_log("is thinking", p, get_time());
	while (die - 10 > get_diff(p))
		usleep(get_diff(p));
}

static void	time_to_eat(t_philo *p)
{
	if (p->id % 2)
	{
		if (pthread_mutex_lock(&p->fork))
			errmsg("pthread_mutex_lock");
	}
	else if (pthread_mutex_lock(p->next))
		errmsg("pthread_mutex_lock");
	if (!(get_bool(&p->tab->starv, &p->tab->starvation)))
		print_log("has taken a fork", p, get_time());
	if (p->id % 2)
	{
		if (pthread_mutex_lock(p->next))
			errmsg("pthread_mutex_lock");
	}
	else if (pthread_mutex_lock(&p->fork))
		errmsg("pthread_mutex_lock");
	if (!(get_bool(&p->tab->starv, &p->tab->starvation)))
		print_log("is eating", p, meal_time(p));
	while (p->tab->time_to_eat > get_diff(p))
		usleep(get_diff(p));
	if (pthread_mutex_unlock(&p->fork))
		errmsg("pthread_mutex_unlock");
	if (pthread_mutex_unlock(p->next))
		errmsg("pthread_mutex_unlock");
}

static void	*diner_solo(t_philo *p)
{
	while (!get_bool(&p->tab->main, &p->tab->start))
		continue ;
	if (pthread_mutex_lock(&p->fork))
		errmsg("pthread_mutex_lock");
	print_log("has taken a fork", p, get_time());
	while (!get_bool(&p->tab->starv, &p->tab->starvation))
		continue ;
	if (pthread_mutex_unlock(&p->fork))
		errmsg("pthread_mutex_unlock");
	return (NULL);
}

void	*diner(void *data)
{
	t_philo	*p;

	p = (t_philo *)data;
	if (p->tab->p_count == 1)
		return (diner_solo(p));
	while (!get_bool(&p->tab->main, &p->tab->start))
		continue ;
	while (!get_bool(&p->tab->starv, &p->tab->starvation))
	{
		time_to_eat(p);
		if (p->tab->must_eat == p->meal
			|| get_bool(&p->tab->starv, &p->tab->starvation))
			break ;
		time_to_sleep(p);
	}
	return (NULL);
}
