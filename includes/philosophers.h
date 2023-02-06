/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpradene <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/06 12:45:13 by lpradene          #+#    #+#             */
/*   Updated: 2023/02/06 12:47:58 by lpradene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <sys/time.h>
# include <pthread.h>
# include <stdbool.h>
# include <stdlib.h>
# include <stdint.h>
# include <stdio.h>

typedef enum e_state
{
	EAT,
	SLEEP,
	THINK
}	t_state;

typedef struct s_philo
{
	uint32_t		n;
	t_state			state;
	pthread_mutex_t	*r_fork;
	pthread_mutex_t	*l_fork;
}	t_philo;

void	destroy(t_philo *philo);
void	init_fork(t_philo *philo, int count);
void	init(t_philo **philo, int count);

#endif
