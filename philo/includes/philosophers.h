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
	THINK,
	DEAD
}	t_state;

typedef struct s_prm
{
	int				count;
	size_t			e_time;
	size_t			s_time;
	size_t			d_time;
	int				rep;
	int				finished;
	int				started;
	bool			dead;
	pthread_mutex_t	m_started;
	pthread_mutex_t	m_finished;
	pthread_mutex_t	m_write;
	pthread_mutex_t	m_dead;
}	t_prm;

typedef struct s_philo
{
	int				id;
	int				eat;
	size_t			last_eat;
	pthread_mutex_t	m_lasteat;
	pthread_mutex_t	*m_rf;
	pthread_mutex_t	*m_lf;
	t_prm			*prm;
}	t_philo;

// INIT
void	destroy(t_philo *philo);
int		init_prm(t_prm *prm, int argc, char **argv);
void	init(t_philo **philo, t_prm *prm);

// TIMESTAMP
size_t	start_time(int n);
size_t	timestamp(void);

// PRINT
void	print(t_philo *philo, t_state state);

// ROUTINE
void	*routine(void *p);

// UTILS
int		ft_atoi(const char *s);
int		overflow(char *str, int n);

#endif
