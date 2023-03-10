/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedperei <pedperei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/04 14:50:58 by pedperei          #+#    #+#             */
/*   Updated: 2023/03/08 15:58:37 by pedperei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"


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
	long int	time;

	time = calc_time();
	while (calc_time() - time < ms)
		usleep(ms / 10);
}

void	print_instruction(int nbr_philo, long int start, long int now, char c)
{
	if (c == 'f')
		printf("%ld %d has taken a fork\n", (now - start), nbr_philo);
	else if (c == 'e')
		printf("%ld %d is eating\n", (now - start), nbr_philo);
	else if (c == 's')
		printf("%ld %d is sleeping\n", (now - start), nbr_philo);
	else if (c == 't')
		printf("%ld %d is thinking\n", (now - start), nbr_philo);
	else if (c == 'd')
		printf("%ld %d died\n", (now - start), nbr_philo);
}

int	eating(t_philo *philo)
{
	pthread_mutex_lock(&philo->info->instruction);
	print_instruction(philo->nbr, philo->info->start, calc_time(), 'e');
	pthread_mutex_unlock(&philo->info->instruction);
	philo->is_eating = 0;
	if (philo->nbr == 1)
		pthread_mutex_unlock(&philo->info->forks[philo->info->nbr_philo - 1]);
	else
		pthread_mutex_unlock(&philo->info->forks[philo->nbr - 2]);
	pthread_mutex_unlock(&philo->info->forks[philo->nbr - 1]);
	philo->last_eat = calc_time();
	return (0);
}

int	take_forks(t_philo *philo)
{
	if (philo->nbr == 1)
	{
		pthread_mutex_lock(&philo->info->forks[philo->info->nbr_philo - 1]);
		print_instruction(philo->nbr, philo->info->start, calc_time(), 'f');
		philo->is_eating++;
	}
	else
	{
		pthread_mutex_lock(&philo->info->forks[philo->nbr - 2]);
		print_instruction(philo->nbr, philo->info->start, calc_time(), 'f');
		philo->is_eating++;
	}
	pthread_mutex_lock(&philo->info->forks[philo->nbr - 1]);
	print_instruction(philo->nbr, philo->info->start, calc_time(), 'f');
	philo->is_eating++;
	if (philo->is_eating == 2)
		return (1);
	else
		return (0);
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
	if (phil->nbr % 2 == 0)
		usleep(phil->info->time_to_eat);
	if (phil->info->times_to_eat != -1)
	{
		while ((phil->nbr_eats < phil->info->times_to_eat) && !philo_dead(phil))
		{
			take_forks(phil);
		}
	}
	else
	{
		while (!philo_dead(phil))
		{
			if(take_forks(phil)==1)
				eating(phil);
			ft_usleep(phil->info->time_to_eat);
			//pthread_detach(phil->info->threads[phil->nbr - 1]);
		}
	}
	return (0);
}

int	init_process(t_philo *philos, t_info *info)
{
	int	i;
	int	*a;

	int j;
	i = 0;
	info->start = calc_time();
	while (i < info->nbr_philo)
	{
		a = &i;
		if (pthread_create(&info->threads[i], NULL, &routine, &philos[*a]) != 0)
			return (0);
		usleep(200);
		i++;
	}
	j = 0;
	while (j < info->nbr_philo)
	{
		while(1)
		{
			if(pthread_join(info->threads[j], NULL) != 0)
			break;
		}
		printf("Terminated %d\n", j);
		j++;
	}
	return (1);
}
