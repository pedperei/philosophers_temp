/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedperei <pedperei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/04 14:18:00 by pedperei          #+#    #+#             */
/*   Updated: 2023/03/04 18:17:05 by pedperei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_info	*init_info(char **argv, int argc)
{
	t_info	*info;

	info = (t_info *)ft_calloc(1, sizeof(t_info));
	if (!info)
		return (0);
	info->nbr_philo = ft_atoi(argv[1]);
	info->time_to_die = ft_atoi(argv[2]);
	info->time_to_eat = ft_atoi(argv[3]);
	info->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		info->times_to_eat = ft_atoi(argv[5]);
	else
		info->times_to_eat = -1;
	gettimeofday(&info->start, NULL);
	pthread_mutex_init(&info->intruction, NULL);
	pthread_mutex_init(&info->die, NULL);
	info->threads = (pthread_t *)malloc((info->nbr_philo + 1)* sizeof(pthread_t));
	return (info);
}

t_philo	**init_philos_mutex(t_philo **philos, t_info *info)
{
	int	i;

	philos = (t_philo **)ft_calloc(info->nbr_philo, sizeof(t_philo *));
	info->forks = (pthread_mutex_t *)ft_calloc(info->nbr_philo,
			sizeof(pthread_mutex_t));
	if (!philos || !info->forks)
		return (0);
	i = 0;
	while (i < info->nbr_philo)
	{
		philos[i] = (t_philo *)ft_calloc(1, sizeof(t_philo));
		if (!philos[i])
			return (0);
		philos[i]->info = info;
		philos[i]->nbr = i + 1;
		philos[i]->time_sleeping = 0;
		philos[i]->time_thinking = 0;
		philos[i]->last_eat = 0;

		pthread_mutex_init(&philos[i]->info->forks[i], NULL);
		i++;
	}
	pthread_mutex_init(&info->intruction, NULL);
	pthread_mutex_init(&info->die, NULL);
	return (philos);
}

int	main(int argc, char **argv)
{
	t_philo **philos;
	t_info *info;

	if (argc != 5 && argc != 6)
	{
		printf("Invalid number of args");
		return (0);
	}
	else
	{
		info = init_info(argv, argc);
		philos = init_philos_mutex(philos, info);
		init_process(philos, info);
	}
}