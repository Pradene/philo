/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   timestamp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpradene <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/07 01:52:05 by lpradene          #+#    #+#             */
/*   Updated: 2023/02/07 01:52:07 by lpradene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

size_t	start_time(int n)
{
	struct timeval	time;
	static size_t	start;

	if (!n)
		return (start);
	gettimeofday(&time, NULL);
	start = time.tv_sec * 1000 + time.tv_usec / 1000;
	return (start);
}

size_t	timestamp(void)
{
	struct timeval	time;
	size_t			timestamp;

	gettimeofday(&time, NULL);
	timestamp = time.tv_sec * 1000 + time.tv_usec / 1000 - start_time(0);
	return (timestamp);
}
