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

typedef struct simulation Simulation;
typedef struct philo Philo;

struct simulation {
	size_t			start_time;
	size_t			time_to_eat;
	size_t			time_to_sleep;
	size_t			time_to_die;
	int				repetition;
	bool			dead;
	pthread_mutex_t	m_started;
	pthread_mutex_t	m_finished;
	pthread_mutex_t	m_write;
	pthread_mutex_t	m_dead;
	Philo			*philos;
	size_t			philos_count;
	size_t			philos_ready;
	size_t			philos_sated;
};

struct philo {
	int				id;
	int				eat;
	size_t			last_eat;
	pthread_mutex_t	m_lasteat;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	*left_fork;
	Simulation		*sim;
};

// INIT
Simulation  *init_simulation(int argc, char **argv);

// TIMESTAMP
size_t	get_time(void);
size_t	simulation_elapsed_time(Simulation *sim);

// PRINT
void	display_philo_state(Philo *philo, PhiloState state);

// ROUTINE
void	*routine(void *p);

void	*simulation_monitor(void *arg);
void	*philo_routine(void *arg);


// LAUNCH
int		launch(Simulation *sim);

// DESTROY
void	destroy_simulation(Simulation *sim);

#endif
