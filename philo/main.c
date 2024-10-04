/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joandre- <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 16:56:31 by joandre-          #+#    #+#             */
/*   Updated: 2024/10/04 02:18:51 by joandre-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	get_int(char *nbr)
{
	int	n;

	n = 0;
	if (nbr)
		while (*nbr)
			n = n * 10 + (*nbr++ - '0');
	return (n);
}

static bool	valid_range(char *nbr)
{
	unsigned int	n;

	if (!nbr)
		return (errmsg("null", NULL), false);
	n = 0;
	while (*nbr)
	{
		n = n * 10 + (*nbr++ - '0');
		if (n > INT_MAX)
			return (errmsg("Number out of range", NULL), false);
	}
	return (true);
}

static bool	table_check(int ac, char **av, t_table *tab)
{
	tab->p_count = get_int(av[1]);
	if (tab->p_count == 0)
		return (errmsg("Can't be 0 philosophers", NULL), (false));
	tab->time_to_die = get_int(av[2]);
	tab->time_to_eat = get_int(av[3]);
	tab->time_to_sleep = get_int(av[4]);
	if (ac == 6)
	{
		tab->must_eat = get_int(av[5]);
		if (tab->must_eat == 0)
			return (errmsg("Can't eat 0 times", NULL), (false));
	}
	return (true);
}

static bool	argv_check(int ac, char **av, t_table *tab)
{
	int	i;
	int	t;

	t = 1;
	while (t < ac)
	{
		i = 0;
		if (!av[t][i])
			return (errmsg("Null string", NULL), false);
		while (av[t][i] >= '0' && av[t][i] <= '9')
			++i;
		if (av[t][i])
			return (errmsg("Numbers only", NULL), false);
		if (!valid_range(av[t++]))
			return (false);
	}
	return (table_check(ac, av, tab));
}

int	main(int argc, char **argv)
{
	t_table	tab;

	if (argc < 5 || argc > 6)
		return (errmsg("Wrong number of arguments", NULL), (-1));
	if (!argv_check(argc, argv, memset(&tab, 0, sizeof(tab))))
		return (-2);
	if (!sim_init(&tab))
		return (1);
	return (sim_exit(&tab));
}
