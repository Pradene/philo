/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpradene <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/10 03:18:56 by lpradene          #+#    #+#             */
/*   Updated: 2023/02/10 03:18:58 by lpradene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

void	print(t_philo *p, t_state state)
{
	static int	i = 1;

	pthread_mutex_lock(&p->param->m_write);
	if (p->param->dead && i)
		printf("%ld %d died\n", timestamp(), p->id + --i);
	else if (state == FORK && !p->param->dead && (p->param->rep == -1
			|| p->param->eat_count / p->param->count < p->param->rep))
		printf("%ld %d has taken a fork\n", timestamp(), p->id);
	else if (state == EAT && !p->param->dead && (p->param->rep == -1
			|| p->param->eat_count / p->param->count < p->param->rep))
		printf("%ld %d is eating\n", timestamp(), p->id);
	else if (state == SLEEP && !p->param->dead && (p->param->rep == -1
			|| p->param->eat_count / p->param->count < p->param->rep))
		printf("%ld %d is sleeping\n", timestamp(), p->id);
	else if (state == THINK && !p->param->dead && (p->param->rep == -1
			|| p->param->eat_count / p->param->count < p->param->rep))
		printf("%ld %d is thinking\n", timestamp(), p->id);
	pthread_mutex_unlock(&p->param->m_write);
}