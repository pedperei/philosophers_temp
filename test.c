/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedperei <pedperei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 14:12:31 by pedperei          #+#    #+#             */
/*   Updated: 2023/01/26 17:39:18 by pedperei         ###   ########.fr       */
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

void create_philo(void)
{
	t_philo *philo;

	philo = (philo *)ft_calloc(1, sizeof(philo));
}

void	*routine(void *philo)
{
	philo = (t_philo *)philo;
	philo =  create_philo();
}

int	main(void)
{
	int n_philo = 100;
	pthread_t philo[n_philo];
	pthread_mutex_t mutex;
	int i;
	int j;
	t_philo **philos;

	i = 0;
	pthread_mutex_init(&mutex, NULL);
	while (i < n_philo)
	{
		if (pthread_create(&philo[i], NULL, &routine, &philos[i]) != 0)
			return (0);
		printf("Started %d\n", i);
		//pthread_mutex_lock(&mutex);
		i++;
		//pthread_mutex_unlock(&mutex);
	}
	i = 0;
	while (i < n_philo)
	{
		if (pthread_join(philo[i], NULL) != 0)
			return (0);
		printf("Terminated %d\n", i);
		//pthread_mutex_lock(&mutex);
		i++;
		//pthread_mutex_unlock(&mutex);
	}
	pthread_mutex_destroy(&mutex);
}