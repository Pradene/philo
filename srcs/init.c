#include "../includes/philo.h"

static bool	is_digit(char c) {
    return (c >= '0' && c <= '9');
}
 
static int	ft_atoi(const char *s) {
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
            return (-1);
		}
    }
    return ((int)n);
}

static int  parse_uint(const char *str, const char *name, int max) {
    int val = ft_atoi(str);
 
    if (val <= 0) {
        fprintf(stderr, "Error: '%s' must be a positive integer\n", name);
        return (-1);
    } else if (max > 0 && val > max) {
        fprintf(stderr, "Error: '%s' must be <= %d\n", name, max);
        return (-1);
    } else {
		return (val);
	}
}

static int  parse_args(Simulation *sim, int argc, char **argv) {
    if (argc != 5 && argc != 6) {
        fprintf(stderr, "Usage: ./philo N_PHILO DIE_TIME EAT_TIME SLEEP_TIME [REP]\n");
        return (-1);
    }

    sim->philos_count  = parse_uint(argv[1], "N_PHILO",   200);
    sim->time_to_die   = parse_uint(argv[2], "DIE_TIME",  0);
    sim->time_to_eat   = parse_uint(argv[3], "EAT_TIME",  0);
    sim->time_to_sleep = parse_uint(argv[4], "SLEEP_TIME",0);
    sim->repetition    = (argc == 6) ? parse_uint(argv[5], "REP", 0) : -1;

	if (sim->philos_count == 0 || sim->time_to_die == 0 || sim->time_to_eat == 0 || sim->time_to_sleep == 0 || sim->repetition == 0) {
        return (-1);
	}

	return (0);
}

static int  init_mutexes(pthread_mutex_t **mutexes, int count) {
    for (int i = 0; i < count; i++) {
        if (pthread_mutex_init(mutexes[i], NULL) != 0) {
            while (--i >= 0) {
                pthread_mutex_destroy(mutexes[i]);
			}
            return (-1);
        }
    }
    return (0);
}

static int  init_simulation_mutexes(Simulation *sim) {
    pthread_mutex_t *mutexes[] = {
        &sim->m_dead,
        &sim->m_started,
        &sim->m_finished,
        &sim->m_write,
    };
    return (init_mutexes(mutexes, 4));
}

static int  init_philo_mutexes(Philo *philo) {
    philo->right_fork = malloc(sizeof(pthread_mutex_t));
    if (!philo->right_fork) {
        return (-1);
	}

    pthread_mutex_t *mutexes[] = {
		philo->right_fork,
		&philo->m_lasteat
	};
    if (init_mutexes(mutexes, 2) != 0) {
        free(philo->right_fork);
		philo->right_fork = NULL;
		return (-1);
	}
    return (0);
}

static void link_forks(Philo *philos, int count) {
    for (int i = 0; i < count; i++) {
        philos[(i + 1) % count].left_fork = philos[i].right_fork;
	}
}

static int	init_philo(Philo *philo, Simulation *sim, int id) {
    if (init_philo_mutexes(philo) != 0) {
        return (-1);
	}
    philo->id       = id;
    philo->sim      = sim;
    philo->eat      = 0;
    philo->last_eat = 0;
    return (0);
}

static Philo	*init_philos(Simulation *sim) {
    Philo *philos = calloc(sim->philos_count, sizeof(Philo));
    if (!philos) {
        return (NULL);
	}

    for (size_t i = 0; i < sim->philos_count; i++) {
        if (init_philo(&philos[i], sim, i + 1) != 0) {
			sim->philos_count = i;
            destroy_simulation(sim);
            return (NULL);
        }
    }
    if (sim->philos_count > 1) {
        link_forks(philos, sim->philos_count);
	}
    return (philos);
}

Simulation	*init_simulation(int argc, char **argv) {
    Simulation *sim = calloc(1, sizeof(Simulation));
    if (sim == NULL) {
        return (NULL);
	}

    if (parse_args(sim, argc, argv) != 0 || init_simulation_mutexes(sim) != 0) {
        free(sim);
        return (NULL);
    }

	sim->philos = init_philos(sim);
	if (!sim->philos) {
		free(sim);
		return (NULL);
	}

    return (sim);
}
