#include "../includes/philo.h"

void	display_philo_state(Philo *p, PhiloState state) {
	pthread_mutex_lock(&p->sim->m_dead);
	if (p->sim->dead) {
		pthread_mutex_unlock(&p->sim->m_dead);
		return ;
	}
	pthread_mutex_unlock(&p->sim->m_dead);
	pthread_mutex_lock(&p->m_lasteat);
	pthread_mutex_lock(&p->sim->m_write);
	switch (state) {	
	case FORK:
		printf("%zu %d has taken a fork\n", timestamp(), p->id);
		break;
	case EAT:
		printf("%zu %d is eating\n", timestamp(), p->id);
		break;
	case SLEEP:
		printf("%zu %d is sleeping\n", timestamp(), p->id);
		break;
	case THINK:
		printf("%zu %d is thinking\n", timestamp(), p->id);
		break;
	default:
		break;
	}
	pthread_mutex_unlock(&p->sim->m_write);
	pthread_mutex_unlock(&p->m_lasteat);
}
