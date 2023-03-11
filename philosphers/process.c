/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedperei <pedperei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/04 14:50:58 by pedperei          #+#    #+#             */
/*   Updated: 2023/03/11 02:21:42 by pedperei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	kill_threads(t_philo *philo)
{
	int	i;

	i = 0;
	while (i < philo->info->nbr_philo)
	{
		pthread_mutex_destroy(&philo->info->forks[i]);
		pthread_detach(philo->info->threads[i]);
		i++;
	}
	pthread_mutex_destroy(&philo->info->instruction);
	return (1);
}

void	print_instruction(t_philo *philo, long int now, char c)
{
	long int	delta;

	delta = now - philo->info->start;
	pthread_mutex_lock(&philo->info->instruction);
	if (c == 'f')
		printf("%ld %d has taken a fork\n", delta, philo->nbr);
	else if (c == 'e')
	{
		printf("%ld %d is eating\n", delta, philo->nbr);
		philo->last_eat = now;
	}
	else if (c == 's')
		printf("%ld %d is sleeping\n", delta, philo->nbr);
	else if (c == 't')
		printf("%ld %d is thinking\n", delta, philo->nbr);
	else if (c == 'd')
	{
		printf("%ld %d died\n", delta, philo->nbr);
		//kill_threads(philo);
	}
	pthread_mutex_unlock(&philo->info->instruction);
}

int	check_philo_eats(t_philo *philo)
{
	int	i;

	i = 0;
	if (philo[i].info->times_to_eat == -1)
		return (0);
	while (i < philo->info->nbr_philo)
	{
		if (philo[i].nbr_eats <= philo[i].info->times_to_eat)
			return (0);
		i++;
	}
	philo->info->reached_limit = 1;
	return (1);
}

int	philo_dead(t_philo *philo)
{
	int	i;

	i = 0;
	while (i < philo->info->nbr_philo)
	{
		if (philo[i].nbr_eats == 0)
		{
			if ((calc_time()
					- philo[i].info->start) < philo[i].info->time_to_die)
				return (0);
			print_instruction(&philo[i], calc_time(), 'd');
			philo->info->any_dead = 1;
			return (1);
		}
		else
		{
			if ((calc_time() - philo[i].last_eat) < philo[i].info->time_to_die)
				return (0);
			print_instruction(&philo[i], calc_time(), 'd');
			philo->info->any_dead = 1;
			return (1);
		}
	}
	return (0);
}

void	*routine(void *arg)
{
	t_philo	*phil;

	phil = (t_philo *)arg;
	while (1)
	{
		take_forks(phil);
		eating(phil);
		sleeping(phil);
		thinking(phil);
	}
	return (0);
}

int	init_process(t_philo *philos, t_info *info)
{
	int	i;
	int	*a;

	i = -1;
	info->start = calc_time();
	while (++i < info->nbr_philo)
		philos[i].last_eat = info->start;
	i = 0;
	while (i < info->nbr_philo)
	{
		a = &i;
		if (pthread_create(&info->threads[i], NULL, &routine, &philos[*a]) != 0)
			return (0);
		usleep(200);
		i++;
	}
	while (!philo_dead(philos) && !check_philo_eats(philos))
		ft_usleep(1);
	kill_threads(philos);
	
	/* i = 0;
	while (i < info->nbr_philo)
	{
		if (pthread_join(info->threads[i], NULL) != 0)
			return (0);
		//printf("Terminated %d\n", j);
		i++;
	}  */
	return (1);
}
