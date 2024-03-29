/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   atoi.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpradene <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/07 14:27:44 by lpradene          #+#    #+#             */
/*   Updated: 2023/02/07 14:29:17 by lpradene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/philosophers.h"

static int	is_digit(char c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

int	ft_atoi(const char *s)
{
	int	i;
	int	n;

	n = 0;
	i = -1;
	while (s && s[++i])
	{
		if (!is_digit(s[i]))
			return (0);
		n = n * 10 + s[i] - 48;
	}
	if (n < 0)
		return (0);
	return (n);
}
