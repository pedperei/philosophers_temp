/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cycle.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedperei <pedperei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 17:30:32 by pedperei          #+#    #+#             */
/*   Updated: 2023/03/12 02:18:09 by pedperei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	eating(t_philo *philo)
{
	print_instruction(philo, calc_time(), 'e');
	philo->is_eating = 0;
	ft_usleep(philo->info->time_to_eat);
	if (philo->nbr == 1)
	{
		pthread_mutex_unlock(&philo->info->forks[philo->nbr - 1]);
		pthread_mutex_unlock(&philo->info->forks[philo->info->nbr_philo - 1]);
	}
	else
	{
		pthread_mutex_unlock(&philo->info->forks[philo->nbr - 2]);
		pthread_mutex_unlock(&philo->info->forks[philo->nbr - 1]);
	}
	pthread_mutex_lock(&philo->info->n_eats);
	philo->nbr_eats++;
	pthread_mutex_unlock(&philo->info->n_eats);
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

int lock_fork(t_philo *philo, int fork_index)
{
	pthread_mutex_lock(&philo->info->forks[fork_index]);
	print_instruction(philo, calc_time(), 'f');
	philo->is_eating++;
	return (1);
}

int	take_forks(t_philo *philo)
{
	if (philo->nbr == 1)
		lock_fork(philo, philo->nbr - 1);
	else
		lock_fork(philo, philo->nbr - 2);
	if (philo->nbr == 1)
		lock_fork(philo, philo->info->nbr_philo - 1);
	else
		lock_fork(philo, philo->nbr - 1);
	if (philo->is_eating == 2)
		return (1);
	else
		return (0);
}
