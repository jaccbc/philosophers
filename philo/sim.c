/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sim.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joandre- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 17:19:57 by joandre-          #+#    #+#             */
/*   Updated: 2024/09/25 17:58:48 by joandre-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool	mutex_init(t_table *tab)
{
	unsigned int	i;

	if (pthread_mutex_init(&tab->log, NULL)
		|| pthread_mutex_init(&tab->main, NULL)
		|| pthread_mutex_init(&tab->starv, NULL))
		return (errmsg("pthread_mutex_init"), false);
	i = 0;
	while (i < tab->p_count)
	{
		if (pthread_mutex_init(&tab->p[i].fork, NULL))
			return (errmsg("pthread_mutex_init"), false);
		if (pthread_mutex_init(&tab->p[i++].diner, NULL))
			return (errmsg("pthread_mutex_init"), false);
	}
	i = 0;
	while (i++ < tab->p_count - 1)
		tab->p[i - 1].next = &tab->p[i].fork;
	tab->p[--i].next = &tab->p[0].fork;
	return (true);
}

static bool	simulation_time(t_table *tab)
{
	unsigned int	i;

	i = 0;
	if (gettimeofday(&tab->startime, NULL))
		return (errmsg("gettimeofday"), free(tab->p), false);
	while (i < tab->p_count)
		tab->p[i++].last_meal = tab->startime;
	set_bool(&tab->main, &tab->start, true);
	return (true);
}

bool	sim_init(t_table *tab)
{
	unsigned int	i;

	tab->p = malloc(tab->p_count * sizeof(t_philo));
	if (tab->p == NULL)
		return (errmsg("malloc"), false);
	if (!mutex_init(tab))
		return (free(tab->p), false);
	if (pthread_create(&tab->host, NULL, monitor, tab))
		return (errmsg("pthread_create"), free(tab->p), false);
	i = 0;
	while (i < tab->p_count)
	{
		tab->p[i].id = i + 1;
		tab->p[i].tab = tab;
		tab->p[i].meal = 0;
		if (pthread_create(&tab->p[i].philo, NULL, diner, &tab->p[i]))
			return (errmsg("pthread_create"), free(tab->p), false);
		++i;
	}
	return (simulation_time(tab));
}

int	sim_exit(t_table *tab)
{
	unsigned int	i;

	if (pthread_join(tab->host, NULL))
		errmsg("pthread_join");
	i = 0;
	while (i < tab->p_count)
		if (pthread_join(tab->p[i++].philo, NULL))
			errmsg("pthread_join");
	if (pthread_mutex_destroy(&tab->log) || pthread_mutex_destroy(&tab->main)
		|| pthread_mutex_destroy(&tab->starv))
		errmsg("pthread_mutex_destroy");
	i = 0;
	while (i < tab->p_count)
	{
		if (pthread_mutex_destroy(&tab->p[i].fork))
			errmsg("pthread_mutex_destroy");
		if (pthread_mutex_destroy(&tab->p[i++].diner))
			errmsg("pthread_mutex_destroy");
	}
	return (free(tab->p), 0);
}
