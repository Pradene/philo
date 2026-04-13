#include "../includes/philo.h"

void	pthread_quit(pthread_t *t, int count) {
	int	i;

	i = -1;
	while (++i < count) {
		if (pthread_join(t[i], NULL)) {
			return ;
		}
	}
}

int	check_death(Simulation *sim, Philo *p) {
	pthread_mutex_lock(&p->m_lasteat);
	if (timestamp() - p->last_eat >= sim->time_to_die) {
		pthread_mutex_unlock(&p->m_lasteat);
		pthread_mutex_lock(&sim->m_write);
		printf("%zu %d died\n", timestamp(), p->id);
		pthread_mutex_unlock(&sim->m_write);
		pthread_mutex_lock(&sim->m_dead);
		sim->dead = true;
		pthread_mutex_unlock(&sim->m_dead);
		return (1);
	}
	pthread_mutex_unlock(&p->m_lasteat);
	return (0);
}

void	monitoring(Simulation *sim, Philo *p) {
	int	i;

	while (1) {
		i = 0;
		while (i < sim->count) {
			pthread_mutex_lock(&sim->m_finished);
			if (sim->finished == sim->count) {
				pthread_mutex_unlock(&sim->m_finished);
				return ;
			}
			pthread_mutex_unlock(&sim->m_finished);
			if (check_death(sim, &p[i])) {
				return ;
			}
			++i;
		}
	}
}

int	launch(Simulation *sim, Philo *p) {
	pthread_t	*t;
	int			i;

	t = malloc(sizeof(pthread_t) * sim->count);
	if (!t) {
		return (1);
	}
	i = 0;
	pthread_mutex_lock(&sim->m_started);
	while (i < sim->count) {
		if (pthread_create(&t[i], NULL, &routine, &p[i])) {
			pthread_mutex_unlock(&sim->m_started);
			pthread_quit(t, i);
			destroy_simulation(p);
			free(t);
			return (1);
		}
		++sim->started;
		++i;
	}
	pthread_mutex_unlock(&sim->m_started);
	monitoring(sim, p);
	pthread_quit(t, sim->count);
	free(t);
	return (0);
}
