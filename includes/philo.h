#ifndef PHILO_H
#define PHILO_H

#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include <stdio.h>

#define MAX_PHILOSOPHERS 200

typedef enum philo_state {
	FORK,
	EAT,
	SLEEP,
	THINK,
	DEAD
}	PhiloState;

typedef struct philo Philo;
typedef struct simulation Simulation;

struct philo {
	size_t			id;
	size_t			eat;
	size_t			last_eat;
	pthread_t		thread;
	pthread_mutex_t	m_lasteat;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	*left_fork;
	Simulation		*sim;
};

struct simulation {
	bool			dead;
	size_t			start_time;
	size_t			time_to_eat;
	size_t			time_to_sleep;
	size_t			time_to_die;
	size_t			repetition;
	pthread_mutex_t	m_write;
	pthread_mutex_t	m_dead;
	size_t			philos_count;
	pthread_mutex_t	forks[MAX_PHILOSOPHERS];
	Philo			philos[MAX_PHILOSOPHERS];
};

bool	init_simulation(Simulation *sim, int argc, char **argv);
bool	launch_simulation(Simulation *sim);
void	destroy_simulation(Simulation *sim);
size_t	simulation_elapsed_time(Simulation *sim);

void	*philo_routine(void *arg);
void	display_philo_state(Philo *philo, PhiloState state);

size_t	get_time(void);
void	wait_until_start_time(Simulation *sim);

#endif
