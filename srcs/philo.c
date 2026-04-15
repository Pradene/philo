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
	lock_fork(p, false);
	pthread_mutex_lock(&p->m_lasteat);
	p->last_eat = get_time();
	p->eat++;
	pthread_mutex_unlock(&p->m_lasteat);
	display_philo_state(p, EAT);
	usleep(1000 * p->sim->time_to_eat);
	pthread_mutex_unlock(p->left_fork);
	pthread_mutex_unlock(p->right_fork);
	return (0);
}

void	*philo_routine(void *p) {
	Philo	*philo = (Philo *)p;

	wait_until_start_time(philo->sim);
	if (philo->sim->philos_count == 1) {
		lock_fork(philo, true);
		pthread_mutex_unlock(philo->right_fork);
		usleep(1000 * philo->sim->time_to_die);
		return (NULL);
	}
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
