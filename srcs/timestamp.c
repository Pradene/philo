#include "../includes/philo.h"

size_t	get_time(void) {
	struct timeval	time;

	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

size_t	simulation_elapsed_time(Simulation *sim) {
	return get_time() - sim->start_time;
}

void	wait_until_start_time(Simulation *sim) {
	while (get_time() < sim->start_time) {
		continue;
	}
}
