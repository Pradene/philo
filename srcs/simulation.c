#include "../includes/philo.h"

void	destroy_simulation(Simulation *sim) {
	pthread_mutex_destroy(&sim->m_write);
	pthread_mutex_destroy(&sim->m_dead);
	for (size_t i = 0; i < sim->philos_count; ++i) {
		Philo philo = sim->philos[i];
		pthread_mutex_destroy(&sim->forks[i]);
		pthread_mutex_destroy(&philo.m_lasteat);
	}
}

static bool	is_philo_dead(Simulation *sim, Philo *p) {
	pthread_mutex_lock(&p->m_lasteat);
    size_t now = simulation_elapsed_time(sim);
	size_t elapsed_time_since_last_eat = get_time() - p->last_eat;
	pthread_mutex_unlock(&p->m_lasteat);
	if (elapsed_time_since_last_eat >= sim->time_to_die) {
		pthread_mutex_lock(&sim->m_write);
		pthread_mutex_lock(&sim->m_dead);
		sim->dead = true;
		printf("%zu %zu died\n", now, p->id);
		pthread_mutex_unlock(&sim->m_dead);
		pthread_mutex_unlock(&sim->m_write);
		return (true);
	}
	return (false);
}

static bool is_philo_sated(Simulation *sim, Philo *p) {
    if (sim->repetition == (size_t)-1) {
        return (false);
	}
    pthread_mutex_lock(&p->m_lasteat);
    size_t eat_count = p->eat;
    pthread_mutex_unlock(&p->m_lasteat);
    return (eat_count >= sim->repetition);
}

static void *simulation_monitor(void *arg) {
    Simulation *sim = (Simulation *)arg;
    size_t     i;

    wait_until_start_time(sim);
    while (true) {
        i = 0;
        size_t sated = 0;
        while (i < sim->philos_count) {
            if (is_philo_sated(sim, &sim->philos[i])) {
                ++sated;
			} else if (is_philo_dead(sim, &sim->philos[i])) {
                return (NULL);
			}
            ++i;
        }
        if (sated == sim->philos_count) {
            pthread_mutex_lock(&sim->m_dead);
            sim->dead = true;
            pthread_mutex_unlock(&sim->m_dead);
            return (NULL);
        }
		usleep(500);
    }
}

static void	pthread_quit(Philo *philos, size_t count) {
	while (count--) {
		if (pthread_join(philos[count].thread, NULL)) {
			return ;
		}
	}
}

bool	launch_simulation(Simulation *sim) {
	pthread_t	monitor;
	size_t		i;

	Philo *philos = sim->philos;
	i = 0;
	while (i < sim->philos_count) {
		if (pthread_create(&philos[i].thread, NULL, &philo_routine, &philos[i])) {
			pthread_quit(philos, i);
			return (1);
		}
		++i;
	}
	if (pthread_create(&monitor, NULL, &simulation_monitor, sim)) {
		pthread_quit(philos, i);
		return (1);
	}
    pthread_join(monitor, NULL);
	pthread_quit(philos, sim->philos_count);
	return (0);
}
