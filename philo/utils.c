/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joandre- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 17:19:11 by joandre-          #+#    #+#             */
/*   Updated: 2024/09/25 17:59:34 by joandre-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_log(const char *log, t_philo *p, struct timeval time)
{
	if (pthread_mutex_lock(&p->tab->log))
		errmsg("pthread_mutex_lock");
	if (!get_bool(&p->tab->starv, &p->tab->starvation))
		printf("%lu %u %s\n",
			((time.tv_sec - p->tab->startime.tv_sec) * 1000)
			+ (time.tv_usec / 1000) - (p->tab->startime.tv_usec / 1000),
			p->id, log);
	if (pthread_mutex_unlock(&p->tab->log))
		errmsg("pthread_mutex_unlock");
}

void	errmsg(const char *msg)
{
	size_t	i;

	if (msg)
	{
		i = 0;
		while (msg[i])
			++i;
		write(STDERR_FILENO, "Error\n", 6);
		write(STDERR_FILENO, msg, i);
		write(STDERR_FILENO, "\n", 1);
	}
}

bool	get_bool(pthread_mutex_t *mtx, bool *ptr)
{
	bool	result;

	if (pthread_mutex_lock(mtx))
		errmsg("pthread_mutex_lock");
	result = *ptr;
	if (pthread_mutex_unlock(mtx))
		errmsg("pthread_mutex_unlock");
	return (result);
}

void	set_bool(pthread_mutex_t *mtx, bool *ptr, bool flag)
{
	if (pthread_mutex_lock(mtx))
		errmsg("pthread_mutex_lock");
	*ptr = flag;
	if (pthread_mutex_unlock(mtx))
		errmsg("pthread_mutex_unlock");
}
