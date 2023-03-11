/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_aux.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedperei <pedperei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/27 19:57:45 by pedperei          #+#    #+#             */
/*   Updated: 2023/03/10 18:01:09 by pedperei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*ft_calloc(size_t nitems, size_t size)
{
	void	*temp;

	temp = (void *)malloc(size * nitems);
	if (!temp)
		return (NULL);
	memset(temp, 0, size);
	return (temp);
}

int	ft_atoi(const char *str)
{
	int	sinal;
	int	res;
	int	i;

	sinal = 1;
	res = 0;
	i = 0;
	while (str[i] == 32 || (str[i] >= 9 && str[i] <= 13))
	{
		i++;
	}
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
		{
			sinal = -1;
		}
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9' && str[i] != '\0')
	{
		res = res * 10 + (str[i] - '0');
		i++;
	}
	return (sinal * res);
}

long int	calc_time(void)
{
	struct timeval	now;
	long int		time;

	gettimeofday(&now, NULL);
	time = (now.tv_sec * 1000 + now.tv_usec / 1000);
	return (time);
}

void	ft_usleep(int ms)
{
	usleep(ms * 1000);
}
