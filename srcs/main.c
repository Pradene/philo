#include "../includes/philo.h"

int	main(int argc, char **argv) {
	Simulation	sim;

	if (!init_simulation(&sim, argc, argv)) {
		return (1);
	}
	if (launch_simulation(&sim)) {
		destroy_simulation(&sim);
		return (1);
	}
	destroy_simulation(&sim);
	return (0);
}
