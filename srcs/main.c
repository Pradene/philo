#include "../includes/philo.h"

int	main(int argc, char **argv) {
	Simulation	*sim;

	sim = init_simulation(argc, argv);
	if (!sim) {
		return (1);
	}
	if (launch_simulation(sim)) {
		destroy_simulation(sim);
		free(sim);
		return (1);
	}
	destroy_simulation(sim);
	free(sim);
	return (0);
}
