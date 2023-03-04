/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedperei <pedperei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/04 14:50:58 by pedperei          #+#    #+#             */
/*   Updated: 2023/03/04 18:28:45 by pedperei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long int	calculate_time(struct timeval start)
{
	struct timeval	now;
	long int		elapsed_time;

	gettimeofday(&now, NULL);
	elapsed_time = 0;
	elapsed_time = (now.tv_sec - start.tv_sec) * 1000;
	elapsed_time += (now.tv_usec - start.tv_usec) / 1000;
	return (elapsed_time);
}

void	print_instruction(int nbr_philo, long int ms, char c)
{
	if (c == 'f')
		printf("%ld %d has taken a fork\n", ms, nbr_philo);
	else if (c == 'e')
		printf("%ld %d is eating\n", ms, nbr_philo);
	else if (c == 's')
		printf("%ld %d is sleeping\n", ms, nbr_philo);
	else if (c == 't')
		printf("%ld %d is thinking\n", ms, nbr_philo);
	else if (c == 'd')
		printf("%ld %d died\n", ms, nbr_philo);
}

int	eating(t_philo *philo)
{
	if(philo->nbr == 1)
	{
		pthread_mutex_lock(&philo->info->forks[philo->info->nbr_philo - 1]);
		print_instruction(philo->nbr,calculate_time(philo->info->start), 'f');
		//pthread_mutex_unlock(&philo->info->forks[philo->info->nbr_philo - 1]);
	}
	else
	{
		pthread_mutex_lock(&philo->info->forks[philo->nbr - 2]);
		print_instruction(philo->nbr,calculate_time(philo->info->start), 'f');
		//pthread_mutex_unlock(&philo->info->forks[philo->nbr - 2]);
	}
	pthread_mutex_lock(&philo->info->forks[philo->nbr - 1]);
	print_instruction(philo->nbr,calculate_time(philo->info->start), 'f');
	//pthread_mutex_unlock(&philo->info->forks[philo->nbr - 1]);
}

int	philo_dead(t_philo *philo)
{
	if (philo->last_eat < philo->info->time_to_die)
	{
		return (0);
	}
	return (1);
}

void	*routine(void *arg)
{
	t_philo	*phil;

	phil = (t_philo *)arg;
	if (phil->info->times_to_eat != -1)
	{
		while ((phil->nbr_eats < phil->info->times_to_eat) && !philo_dead(phil))
		{
			eating(phil);
		}
	}
	else
	{
		while (!philo_dead(phil))
		{
			eating(phil);
		}
	}
}

int	init_process(t_philo **philos, t_info *info)
{
	int i;
	int j;
	int *a;

	i = 0;
	gettimeofday(&info->start, NULL);
	while (i < info->nbr_philo)
	{
		a = &i;
		if (pthread_create(&info->threads[i], NULL, &routine, philos[*a]) != 0)
			return (0);
		usleep(10000);
		i++;
	}
	/* j = 0;
	while (j < info->nbr_philo)
	{
		if (pthread_join(threads[j], NULL) != 0)
			return (0);
		printf("Terminated %d\n", j);
		j++;
	} */
	return (1);
}
