/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpradene <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/06 12:46:38 by lpradene          #+#    #+#             */
/*   Updated: 2023/02/06 12:47:39 by lpradene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

void	*print(void *philo)
{
	t_philo	*p;

	p = (t_philo *)philo;
	printf("N: %d\nRight: %p\nLeft: %p\n", p->n, p->r_fork, p->l_fork);
	return (NULL);
}

void	launch(t_philo *philo, int count)
{
	pthread_t	*t;
	int			i;

	t = malloc(sizeof(pthread_t) * count);
	i = -1;
	while (++i < count)
		pthread_create(&t[i], NULL, &print, &philo[i]);
	i = -1;
	while (++i < count)
		pthread_join(t[i], NULL);
	free(t);
}

int	main(int argc, char **argv)
{
	int			count;
	t_philo		*philo;

	count = 10;
	init(&philo, count);
	init_fork(philo, count);
    launch(philo, count);
	destroy(philo);
	return (0);
}
