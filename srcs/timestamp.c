#include "../includes/philo.h"

size_t	start_time(int n) {
	struct timeval	time;
	static size_t	start;

	if (!n) {
		return (start);
	}

	gettimeofday(&time, NULL);
	start = time.tv_sec * 1000 + time.tv_usec / 1000;
	return (start);
}

size_t	timestamp(void) {
	struct timeval	time;
	size_t			timestamp;

	gettimeofday(&time, NULL);
	timestamp = time.tv_sec * 1000 + time.tv_usec / 1000 - start_time(0);
	return (timestamp);
}
