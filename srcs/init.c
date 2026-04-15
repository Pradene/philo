#include "../includes/philo.h"

static bool	is_digit(char c) {
    return (c >= '0' && c <= '9');
}
 
static int args_to_uint(const char *s) {
    long    n;
    int     i;
 
    n = 0;
    i = 0;
    while (s && s[i]) {
        if (!is_digit(s[i])) {
            return (0);
		}
        n = n * 10 + s[i++] - '0';
        if (n > INT_MAX) {
            return (0);
		}
    }
    return ((int)n);
}

static int parse_uint(const char *str, const char *name, int max) {
    int val = args_to_uint(str);
 
    if (val == 0) {
        fprintf(stderr, "Error: '%s' must be a positive integer\n", name);
        return (0);
    } else if (max > 0 && val > max) {
        fprintf(stderr, "Error: '%s' must be <= %d\n", name, max);
        return (0);
    } else {
		return (val);
	}
}

static bool parse_args(Simulation *sim, int argc, char **argv) {
    if (argc != 5 && argc != 6) {
        fprintf(stderr, "Usage: ./philo N_PHILO DIE_TIME EAT_TIME SLEEP_TIME [REP]\n");
        return (false);
    }

    sim->philos_count  = parse_uint(argv[1], "N_PHILO",   200);
    sim->time_to_die   = parse_uint(argv[2], "DIE_TIME",  0);
    sim->time_to_eat   = parse_uint(argv[3], "EAT_TIME",  0);
    sim->time_to_sleep = parse_uint(argv[4], "SLEEP_TIME",0);
    sim->repetition    = (argc == 6) ? parse_uint(argv[5], "REP", 0) : -1;

	if (sim->philos_count == 0 || sim->time_to_die == 0 || sim->time_to_eat == 0 || sim->time_to_sleep == 0 || sim->repetition == 0) {
        return (false);
	}

	return (true);
}

static bool init_mutexes(pthread_mutex_t **mutexes, int count) {
    for (int i = 0; i < count; i++) {
        if (pthread_mutex_init(mutexes[i], NULL) != 0) {
            while (i--) {
                pthread_mutex_destroy(mutexes[i]);
			}
            return (false);
        }
    }
    return (true);
}

static bool init_simulation_mutexes(Simulation *sim) {
    pthread_mutex_t *mutexes[] = {
        &sim->m_dead,
        &sim->m_write,
    };
    return (init_mutexes(mutexes, 2));
}

static bool  init_philo_mutexes(Philo *philo) {
    pthread_mutex_t *mutexes[] = {
		&philo->m_lasteat
	};

    if (!init_mutexes(mutexes, 1)) {
		return (false);
	}

    return (true);
}

static bool	init_philo(Philo *philo, Simulation *sim, int id) {
    if (!init_philo_mutexes(philo)) {
        return (false);
	}

    philo->id         = id + 1;
    philo->sim        = sim;
    philo->eat        = 0;
    philo->last_eat   = sim->start_time;
    philo->right_fork = &sim->forks[id];
    philo->left_fork  = &sim->forks[(id + 1) % sim->philos_count];
    return (true);
}

bool    init_simulation(Simulation *sim, int argc, char **argv) {
    if (!parse_args(sim, argc, argv) || !init_simulation_mutexes(sim)) {
        return (false);
    }

    sim->start_time = get_time() + sim->philos_count * 10;
    for (size_t i = 0; i < sim->philos_count; i++) {
        if (pthread_mutex_init(&sim->forks[i], NULL) != 0) {
            sim->philos_count = i;
            destroy_simulation(sim);
            return (false);
        }
        if (!init_philo(&sim->philos[i], sim, i)) {
			sim->philos_count = i;
            destroy_simulation(sim);
            return (false);
        }
    }

    return (true);
}
