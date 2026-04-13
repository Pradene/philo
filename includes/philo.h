#ifndef PHILO_H
# define PHILO_H

# include <sys/time.h>
# include <unistd.h>
# include <pthread.h>
# include <stdbool.h>
# include <stdlib.h>
# include <stdint.h>
# include <limits.h>
# include <stdio.h>

typedef enum philo_state {
	FORK,
	EAT,
	SLEEP,
	THINK,
	DEAD
}	PhiloState;

typedef struct simulation {
	int				count;
	size_t			time_to_eat;
	size_t			time_to_sleep;
	size_t			time_to_die;
	int				rep;
	int				finished;
	int				started;
	bool			dead;
	pthread_mutex_t	m_started;
	pthread_mutex_t	m_finished;
	pthread_mutex_t	m_write;
	pthread_mutex_t	m_dead;
}	Simulation;

typedef struct philo {
	int				id;
	int				eat;
	size_t			last_eat;
	pthread_mutex_t	m_lasteat;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	*left_fork;
	Simulation			*sim;
}	Philo;

// INIT
Philo   	*init_philos(Simulation *s);
Simulation  *init_simulation(int argc, char **argv);

// TIMESTAMP
size_t	start_time(int n);
size_t	timestamp(void);

// PRINT
void	display_philo_state(Philo *philo, PhiloState state);

// ROUTINE
void	*routine(void *p);

// LAUNCH
int		launch(Simulation *sim, Philo *p);

// DESTROY
void	destroy_philos(Philo *philo, int count);
void	destroy_simulation(Philo *philo);

#endif
