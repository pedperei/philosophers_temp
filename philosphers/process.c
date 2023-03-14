/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedperei <pedperei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/04 14:50:58 by pedperei          #+#    #+#             */
/*   Updated: 2023/03/14 01:32:32 by pedperei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	kill_threads(t_philo *philo)
{
	int	i;

	i = 0;
	while (i < philo->info->nbr_philo)
	{
		//pthread_mutex_destroy(&philo->info->forks[i]);
		pthread_detach(philo->info->threads[i]);
		i++;
	}
	//pthread_mutex_destroy(&philo->info->instruction);
	return (1);
}

void	print_instruction(t_philo *philo, long int now, char c)
{
	long int	delta;

	delta = now - philo->info->start;
	pthread_mutex_lock(&philo->info->instruction);
	pthread_mutex_lock(&philo->info->crit);
	if (philo->info->reached_limit != 1 && philo->info->any_dead != 1)
	{
		if (c == 'f')
			printf("%ld %d has taken a fork\n", delta, philo->nbr);
		else if (c == 'e')
		{
			printf("%ld %d is eating\n", delta, philo->nbr);
			pthread_mutex_lock(&philo->info->l_eat);
			philo->last_eat = now;
			pthread_mutex_unlock(&philo->info->l_eat);
		}
		else if (c == 's')
			printf("%ld %d is sleeping\n", delta, philo->nbr);
		else if (c == 't')
			printf("%ld %d is thinking\n", delta, philo->nbr);
		else if (c == 'd')
			printf("%ld %d died\n", delta, philo->nbr);
	}
	pthread_mutex_unlock(&philo->info->crit);
	pthread_mutex_unlock(&philo->info->instruction);
}

int	check_philo_eats(t_philo *philo)
{
	int	i;
	int	nbr_eats;

	i = 0;
	if (philo[i].info->times_to_eat == -1)
		return (0);
	while (i < philo->info->nbr_philo)
	{
		pthread_mutex_lock(&philo->info->n_eats);
		nbr_eats = philo[i].nbr_eats;
		pthread_mutex_unlock(&philo->info->n_eats);
		if (nbr_eats < philo[i].info->times_to_eat)
			return (0);
		i++;
	}
	pthread_mutex_lock(&philo->info->crit);
	philo->info->reached_limit = 1;
	pthread_mutex_unlock(&philo->info->crit);
	return (1);
}

int	philo_dead(t_philo *philo)
{
	int			i;
	int			nbr_eats;
	long int	last_eat;

	i = 0;
	pthread_mutex_lock(&philo->info->n_eats);
	nbr_eats = philo[i].nbr_eats;
	pthread_mutex_unlock(&philo->info->n_eats);
	pthread_mutex_lock(&philo->info->l_eat);
	last_eat = philo[i].last_eat;
	pthread_mutex_unlock(&philo->info->l_eat);
	while (i < philo->info->nbr_philo)
	{
		if (nbr_eats == 0)
		{
			if ((calc_time()
					- philo[i].info->start) < philo[i].info->time_to_die)
				return (0);
			print_instruction(&philo[i], calc_time(), 'd');
			pthread_mutex_lock(&philo->info->crit);
			philo->info->any_dead = 1;
			pthread_mutex_unlock(&philo->info->crit);
			return (1);
		}
		else
		{
			if ((calc_time() - last_eat) < philo[i].info->time_to_die)
				return (0);
			print_instruction(&philo[i], calc_time(), 'd');
			pthread_mutex_lock(&philo->info->crit);
			philo->info->any_dead = 1;
			pthread_mutex_unlock(&philo->info->crit);
			return (1);
		}
	}
	return (0);
}

void	*routine(void *arg)
{
	t_philo	*phil;

	phil = (t_philo *)arg;
	if (phil->nbr % 2 == 0)
		ft_usleep(1);
	while (1)
	{
		take_forks(phil);
		eating(phil);
		sleeping(phil);
		thinking(phil);
		pthread_mutex_lock(&phil->info->crit);
		if (phil->info->reached_limit == 1 || phil->info->any_dead == 1)
		{
			pthread_mutex_unlock(&phil->info->crit);
			break ;
		}
		pthread_mutex_unlock(&phil->info->crit);
	}
	return (0);
}

void	*one_philo(void *arg)
{
	t_philo	*phil;
	long start;
	int i;
	
	phil = (t_philo *)arg;
	pthread_mutex_lock(&phil->info->init);
	start = phil->info->start;
	pthread_mutex_unlock(&phil->info->init);
	i = 0;
	if (start == -1)
	{
		pthread_mutex_lock(&phil->info->init);
		phil->info->start = calc_time();
		pthread_mutex_unlock(&phil->info->init);
		phil[i].last_eat = start;
	}
	print_instruction(phil, calc_time(), 'f');
	ft_usleep(phil->info->time_to_die);
	print_instruction(phil, calc_time(), 'd');
	return (0);
}

int	init_process(t_philo *philos, t_info *info)
{
	int	i;
	int	*a;

	i = 0;
	if (info->nbr_philo == 1)
	{
		a = &i;
		if (pthread_create(&info->threads[i], NULL, &one_philo,
				&philos[*a]) != 0)
			return (0);
	}
	else
	{
		while (i < info->nbr_philo)
		{
			a = &i;
			if (pthread_create(&info->threads[i], NULL, &routine,
					&philos[*a]) != 0)
				return (0);
			ft_usleep(1);
			i++;
		}
	}
	ft_usleep(60);
	while (1)
	{
		if (philo_dead(philos) || check_philo_eats(philos))
			break ;
	}
	return (1);
}
