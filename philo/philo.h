/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joandre- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 17:13:35 by joandre-          #+#    #+#             */
/*   Updated: 2024/10/04 02:44:03 by joandre-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <stdbool.h>
# include <limits.h>
# include <string.h>
# include <pthread.h>
# include <sys/time.h>

typedef struct s_philo	t_philo;

typedef struct s_table
{
	struct timeval	startime;
	bool			start;
	bool			starvation;
	time_t			time_to_die;
	time_t			time_to_eat;
	time_t			time_to_sleep;
	unsigned int	p_count;
	unsigned int	must_eat;
	pthread_mutex_t	main;
	pthread_mutex_t	starv;
	pthread_mutex_t	log;
	pthread_t		host;
	t_philo			*p;
}	t_table;

typedef struct s_philo
{
	unsigned int	id;
	unsigned int	meal;
	struct timeval	last_meal;
	pthread_t		philo;
	pthread_mutex_t	diner;
	pthread_mutex_t	fork;
	pthread_mutex_t	*next;
	t_table			*tab;
}	t_philo;

struct timeval	get_time(t_table *tab);
struct timeval	get_last_meal(t_philo *p);
time_t			get_diff(t_philo *p);
void			print_log(const char *log, t_philo *p, struct timeval time);
void			errmsg(const char *msg, t_table *tab);
void			set_bool(pthread_mutex_t *mtx, bool *ptr, bool flag,
					t_table *tab);
bool			get_bool(pthread_mutex_t *mtx, bool *ptr, t_table *tab);
void			*diner(void *data);
void			*monitor(void *data);
bool			sim_init(t_table *tab);
int				sim_exit(t_table *tab);

#endif
