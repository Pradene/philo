#include "../includes/philo.h"

static void	lock_fork(Philo *p, bool is_first) {
	if (p->id % 2) {
		if (is_first)	pthread_mutex_lock(p->right_fork);
		else			pthread_mutex_lock(p->left_fork);

	} else {
		if (is_first)	pthread_mutex_lock(p->left_fork);
		else			pthread_mutex_lock(p->right_fork);
	}
	display_philo_state(p, FORK);
}

static int	eat(Philo *p) {
	lock_fork(p, true);
	if (p->sim->philos_count == 1) {
		pthread_mutex_unlock(p->right_fork);
		usleep(1000 * p->sim->time_to_die);
		return (1);
	}
	lock_fork(p, false);
	pthread_mutex_lock(&p->m_lasteat);
	p->last_eat = simulation_elapsed_time(p->sim);
	pthread_mutex_unlock(&p->m_lasteat);
	display_philo_state(p, EAT);
	p->eat++;
	usleep(1000 * p->sim->time_to_eat);
	pthread_mutex_unlock(p->left_fork);
	pthread_mutex_unlock(p->right_fork);
	if (p->eat == p->sim->repetition) {
		pthread_mutex_lock(&p->sim->m_finished);
		p->sim->philos_sated++;
		pthread_mutex_unlock(&p->sim->m_finished);
		return (1);
	}
	return (0);
}

static void	wait_until_philos_ready(Philo *philo) {
    while (true) {
        pthread_mutex_lock(&philo->sim->m_started);
        if (philo->sim->philos_ready >= philo->sim->philos_count) {
            pthread_mutex_unlock(&philo->sim->m_started);
            return ;
        }
        pthread_mutex_unlock(&philo->sim->m_started);
        usleep(100);
    }
}

void	*philo_routine(void *p) {
	Philo	*philo = (Philo *)p;

	wait_until_philos_ready(philo);
	if (philo->id % 2 == 0) {
		usleep(1000 * philo->sim->time_to_eat / 2);
	}
	while (1) {
		pthread_mutex_lock(&philo->sim->m_dead);
		if (philo->sim->dead) {
			pthread_mutex_unlock(&philo->sim->m_dead);
			break ;
		}
		pthread_mutex_unlock(&philo->sim->m_dead);
		if (eat(philo)) {
			break ;
		}
		display_philo_state(philo, SLEEP);
		usleep(1000 * philo->sim->time_to_sleep);
		display_philo_state(philo, THINK);
		if (philo->sim->philos_count % 2) {
			usleep(1000 * philo->sim->time_to_eat / 2);
		}
	}
	return (NULL);
}
