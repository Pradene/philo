#include "../includes/philo.h"

static int  is_digit(char c) {
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

static int  parse_positive_int(const char *str, const char *name, int max) {
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

static int  parse_args(Simulation *s, int argc, char **argv) {
    if (argc != 5 && argc != 6) {
        fprintf(stderr, "Usage: ./philo N_PHILO DIE_TIME EAT_TIME SLEEP_TIME [REP]\n");
        return (-1);
    }

    s->count         = parse_positive_int(argv[1], "N_PHILO",   200);
    s->time_to_die   = parse_positive_int(argv[2], "DIE_TIME",  0);
    s->time_to_eat   = parse_positive_int(argv[3], "EAT_TIME",  0);
    s->time_to_sleep = parse_positive_int(argv[4], "SLEEP_TIME",0);
    s->rep           = (argc == 6) ? parse_positive_int(argv[5], "REP", 0) : -1;

	if (s->count < 0 || s->time_to_die < 0 || s->time_to_eat < 0 || s->time_to_sleep < 0 || s->rep == 0) {
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

static int  init_simulation_mutexes(Simulation *s) {
    pthread_mutex_t *mutexes[] = {
        &s->m_dead,
        &s->m_started,
        &s->m_finished,
        &s->m_write,
    };
    return (init_mutexes(mutexes, 4));
}

static int  init_philo_mutexes(Philo *p) {
    p->right_fork = malloc(sizeof(pthread_mutex_t));
    if (!p->right_fork) {
        return (-1);
	}

    pthread_mutex_t *mutexes[] = {
		p->right_fork,
		&p->m_lasteat
	};
    if (init_mutexes(mutexes, 2) != 0) {
        free(p->right_fork);
		p->right_fork = NULL;
		return (-1);
	}
    return (0);
}

static void link_forks(Philo *philos, int count) {
    for (int i = 0; i < count; i++) {
        philos[(i + 1) % count].left_fork = philos[i].right_fork;
	}
}

static int  init_philo(Philo *p, Simulation *s, int id) {
    if (init_philo_mutexes(p) != 0) {
        return (-1);
	}
    p->id       = id + 1;
    p->sim      = s;
    p->eat      = 0;
    p->last_eat = timestamp();
    return (0);
}

Philo   *init_philos(Simulation *s) {
    Philo *philos = calloc(s->count, sizeof(Philo));
    if (!philos) {
        return (NULL);
	}

    for (int i = 0; i < s->count; i++) {
        if (init_philo(&philos[i], s, i) != 0) {
            destroy_philos(philos, i);
            return (NULL);
        }
    }
    if (s->count > 1) {
        link_forks(philos, s->count);
	}
    return (philos);
}

Simulation  *init_simulation(int argc, char **argv) {
    Simulation *sim = calloc(1, sizeof(Simulation));
    if (sim == NULL) {
        return (NULL);
	}

    if (parse_args(sim, argc, argv) != 0 || init_simulation_mutexes(sim) != 0) {
        free(sim);
        return (NULL);
    }

    return (sim);
}
