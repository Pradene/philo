#include "../includes/philo.h"

int	main(int argc, char **argv) {
	Simulation	*sim;
	Philo		*philos;

	sim = init_simulation(argc, argv);
	if (!sim) {
		return (1);
	}
	start_time(1);
	philos = init_philos(sim);
	if (!philos) {
		return (1);
	}
	if (launch(sim, philos)) {
		return (1);
	}
	destroy_simulation(philos);
	free(sim);
	return (0);
}
