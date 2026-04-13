#include "../includes/philo.h"

static void	wait(Philo *p, size_t time) {
	size_t	i;

	i = 0;
	while (++i <= time / 10) {
		usleep(1 * 1000 * 10);
		pthread_mutex_lock(&p->sim->m_dead);
		if (p->sim->dead) {
			pthread_mutex_unlock(&p->sim->m_dead);
			return ;
		}
		pthread_mutex_unlock(&p->sim->m_dead);
	}
}

static void	lock_fork(Philo *p, int n) {
	if (p->id % 2) {
		if (n) {
			pthread_mutex_lock(p->right_fork);
		} else {
			pthread_mutex_lock(p->left_fork);
		}
	} else {
		if (n) {
			pthread_mutex_lock(p->left_fork);
		} else {
			pthread_mutex_lock(p->right_fork);
		}
	}
	display_philo_state(p, FORK);
}

static int	eat(Philo *p) {
	lock_fork(p, 1);
	if (p->sim->count == 1) {
		pthread_mutex_unlock(p->right_fork);
		wait(p, p->sim->time_to_die);
		return (1);
	}
	lock_fork(p, 0);
	pthread_mutex_lock(&p->m_lasteat);
	p->last_eat = timestamp();
	pthread_mutex_unlock(&p->m_lasteat);
	display_philo_state(p, EAT);
	p->eat++;
	wait(p, p->sim->time_to_eat);
	pthread_mutex_unlock(p->left_fork);
	pthread_mutex_unlock(p->right_fork);
	if (p->eat == p->sim->rep) {
		pthread_mutex_lock(&p->sim->m_finished);
		p->sim->finished++;
		pthread_mutex_unlock(&p->sim->m_finished);
		return (1);
	}
	return (0);
}
void	wait_threads(Philo *philo) {
    while (true) {
        pthread_mutex_lock(&philo->sim->m_started);
        if (philo->sim->started >= philo->sim->count) {
            pthread_mutex_unlock(&philo->sim->m_started);
            return ;
        }
        pthread_mutex_unlock(&philo->sim->m_started);
        usleep(100);
    }
}

void	*routine(void *p) {
	Philo	*philo = (Philo *)p;

	wait_threads(philo);
	if ((philo->id + 1) % 2) {
		wait(philo, philo->sim->time_to_eat / 2);
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
		wait(philo, philo->sim->time_to_sleep);
		display_philo_state(philo, THINK);
		if (philo->sim->count % 2) {
			wait(philo, philo->sim->time_to_eat / 2);
		}
	}
	return (NULL);
}
