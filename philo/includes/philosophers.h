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

typedef enum e_state
{
	FORK,
	EAT,
	SLEEP,
	THINK
}	t_state;

typedef struct s_param
{
	int32_t			count;
	uint32_t		e_time;
	uint32_t		s_time;
	uint32_t		d_time;
	int32_t			eat_count;
	int32_t			rep;
	bool			dead;
	pthread_mutex_t	m_write;
	pthread_mutex_t	m_dead;
	pthread_mutex_t	m_eat;
}	t_param;

typedef struct s_philo
{
	uint32_t		id;
	size_t			last_eat;
	pthread_mutex_t	*m_rf;
	pthread_mutex_t	*m_lf;
	t_param			*param;
}	t_philo;

// INIT
void	destroy(t_philo *philo);
void	init(t_philo **philo, t_param *param, int argc, char **argv);

// TIMESTAMP
size_t	start_time(int n);
size_t	timestamp(void);

// PRINT
void	print(t_philo *philo, t_state state);

// ROUTINE
void	*routine(void *philo);

// UTILS
int		ft_atoi(const char *s);
void	error(char *s);

#endif
