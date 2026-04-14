#include "../includes/philo.h"

void	destroy_simulation(Simulation *sim) {
	pthread_mutex_destroy(&sim->m_write);
	pthread_mutex_destroy(&sim->m_finished);
	pthread_mutex_destroy(&sim->m_started);
	pthread_mutex_destroy(&sim->m_dead);
	for (size_t i = 0; i < sim->philos_count; ++i) {
		Philo philo = sim->philos[i];
		pthread_mutex_destroy(philo.right_fork);
		pthread_mutex_destroy(&philo.m_lasteat);
		free(philo.right_fork);
	}
	free(sim->philos);
}

static bool	is_philo_dead(Simulation *sim, Philo *p) {
	pthread_mutex_lock(&p->m_lasteat);
    size_t now = simulation_elapsed_time(sim);
	size_t elapsed_time_since_last_eat = now - p->last_eat;
	pthread_mutex_unlock(&p->m_lasteat);
	if (elapsed_time_since_last_eat >= sim->time_to_die) {
		pthread_mutex_lock(&sim->m_write);
		printf("%zu %zu died\n", now, p->id);
		pthread_mutex_unlock(&sim->m_write);
		pthread_mutex_lock(&sim->m_dead);
		sim->dead = true;
		pthread_mutex_unlock(&sim->m_dead);
		return (true);
	}
	return (false);
}

static void	*simulation_monitor(void *arg) {
	Simulation	*sim = (Simulation *)arg;
	Philo		*philos = sim->philos;
	size_t		i;

	while (true) {
		i = 0;
		while (i < sim->philos_count) {
			pthread_mutex_lock(&sim->m_finished);
			if (sim->philos_sated == sim->philos_count) {
				pthread_mutex_unlock(&sim->m_finished);
				return (NULL);
			}
			pthread_mutex_unlock(&sim->m_finished);
			if (is_philo_dead(sim, &philos[i])) {
				return (NULL);
			}
			++i;
		}
	}
}

static void	pthread_quit(pthread_t *threads, size_t count) {
	while (count--) {
		if (pthread_join(threads[count], NULL)) {
			return ;
		}
	}
}

bool	launch_simulation(Simulation *sim) {
	pthread_t	monitor;
	pthread_t	*threads;
	size_t		i;

	Philo *philos = sim->philos;
	threads = malloc(sizeof(pthread_t) * sim->philos_count);
	if (!threads) {
		return (1);
	}
	i = 0;
	pthread_mutex_lock(&sim->m_started);
	while (i < sim->philos_count) {
		if (pthread_create(&threads[i], NULL, &philo_routine, &philos[i])) {
			pthread_mutex_unlock(&sim->m_started);
			pthread_quit(threads, i);
			free(threads);
			return (1);
		}
		++sim->philos_ready;
		++i;
	}
	sim->start_time = get_time();
	pthread_mutex_unlock(&sim->m_started);
	if (pthread_create(&monitor, NULL, &simulation_monitor, sim)) {
		pthread_quit(threads, i);
		free(threads);
		return (1);
	}
    pthread_join(monitor, NULL);
	pthread_quit(threads, sim->philos_count);
	free(threads);
	return (0);
}
