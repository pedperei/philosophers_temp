/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cycle.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedperei <pedperei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 17:30:32 by pedperei          #+#    #+#             */
/*   Updated: 2023/03/10 19:01:08 by pedperei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	eating(t_philo *philo)
{
	print_instruction(philo, calc_time(), 'e');
	philo->is_eating = 0;
	ft_usleep(philo->info->time_to_eat);
	if (philo->nbr == 1)
		pthread_mutex_unlock(&philo->info->forks[philo->info->nbr_philo - 1]);
	else
		pthread_mutex_unlock(&philo->info->forks[philo->nbr - 2]);
	pthread_mutex_unlock(&philo->info->forks[philo->nbr - 1]);
	philo->nbr_eats++;
	return (1);
}

int	sleeping(t_philo *philo)
{
	print_instruction(philo, calc_time(), 's');
	ft_usleep(philo->info->time_to_sleep);
	return (1);
}

int	thinking(t_philo *philo)
{
	print_instruction(philo, calc_time(), 't');
	ft_usleep(philo->info->time_to_think);
	return (1);
}

int	take_forks(t_philo *philo)
{
	if (philo->nbr == 1)
	{
		pthread_mutex_lock(&philo->info->forks[philo->info->nbr_philo - 1]);
		print_instruction(philo, calc_time(), 'f');
		philo->is_eating++;
	}
	else
	{
		pthread_mutex_lock(&philo->info->forks[philo->nbr - 2]);
		print_instruction(philo, calc_time(), 'f');
		philo->is_eating++;
	}
	pthread_mutex_lock(&philo->info->forks[philo->nbr - 1]);
	print_instruction(philo, calc_time(), 'f');
	philo->is_eating++;
	if (philo->is_eating == 2)
		return (1);
	else
		return (0);
}