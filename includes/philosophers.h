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
# include <unistd.h>
# include <pthread.h>
# include <stdbool.h>
# include <stdlib.h>
# include <stdint.h>
# include <stdio.h>

typedef struct s_time
{
	uint32_t	die;
	uint32_t	eat;
	uint32_t	sleep;
}	t_time;

typedef struct s_philo
{
	uint32_t		n;
	int32_t			i;
	t_time			time;
	size_t			last_eat;
	pthread_mutex_t	*rf;
	pthread_mutex_t	*lf;
}	t_philo;

// INIT
void	destroy(t_philo *philo, int count);
void	init_fork(t_philo *philo, int count);
void	init(t_philo **philo, t_time time, int count, int i);

// TIMESTAMP
size_t	timestamp(void);

#endif
