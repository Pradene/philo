/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   oveflow.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lpradene <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/07 14:28:11 by lpradene          #+#    #+#             */
/*   Updated: 2023/02/07 14:28:13 by lpradene         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/philosophers.h"

static int	absolute(int n)
{
	if (n < 0)
		return (-n);
	return (n);
}

static int	count(int n)
{
	int	c;
	int	d;

	c = 0;
	d = 1;
	if (n < 0)
		c++;
	while (n || d)
	{
		n /= 10;
		d = 0;
		c++;
	}
	return (c);
}

char	*ft_itoa(int n)
{
	int		c;
	char	*str;
	int		d;

	d = 1;
	c = count(n);
	str = malloc(sizeof(char) * (c + 1));
	if (!str)
		return (0);
	str[c] = '\0';
	if (n < 0)
		*str = '-';
	while (n || d)
	{
		d = 0;
		c--;
		str[c] = absolute(n % 10) + 48;
		n /= 10;
	}
	return (str);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i] || s2[i])
	{
		if ((unsigned char)s1[i] != (unsigned char)s2[i])
			return (1);
		i++;
	}
	return (0);
}

int	overflow(char *str, int n)
{
	char	*s;
	int		result;

	s = ft_itoa(n);
	result = ft_strcmp(str, s);
	free(s);
	return (result);
}
