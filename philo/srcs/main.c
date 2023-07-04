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

void	launch(t_philo *philo)
{
	pthread_t	*t;
	int			i;
	int			count;

	count = philo->prm->count;
	t = malloc(sizeof(pthread_t) * count);
	if (!t)
		return ;
	i = -1;
	while (++i < count)
		if (pthread_create(&t[i], NULL, &routine, &philo[i]))
			exit(EXIT_FAILURE);
	i = -1;
	while (++i < count)
		if (pthread_join(t[i], NULL))
			exit(EXIT_FAILURE);
	free(t);
}

int	main(int argc, char **argv)
{
	t_prm		prm;
	t_philo		*philo;

	start_time(1);
	init(&philo, &prm, argc, argv);
	launch(philo);
	destroy(philo);
	return (0);
}
