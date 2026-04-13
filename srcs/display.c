#include "../includes/philo.h"

static const char *philo_state_as_string(PhiloState state) {
	switch (state) {	
	case FORK:  return ("has taken a fork");
	case EAT:   return ("is eating");
	case SLEEP: return ("is sleeping");
	case THINK: return ("is thinking");
	default:    return ("");
	}
}

void	display_philo_state(Philo *p, PhiloState state) {
	Simulation *sim = (Simulation *)p->sim;
	
	pthread_mutex_lock(&sim->m_dead);
	if (sim->dead) {
		pthread_mutex_unlock(&sim->m_dead);
		return ;
	}
	pthread_mutex_unlock(&sim->m_dead);
	pthread_mutex_lock(&sim->m_write);
	printf("%zu %d %s\n", simulation_elapsed_time(sim), p->id, philo_state_as_string(state));
	pthread_mutex_unlock(&sim->m_write);
}
