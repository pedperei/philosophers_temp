/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pedperei <pedperei@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 14:10:22 by pedperei          #+#    #+#             */
/*   Updated: 2023/03/07 00:03:06 by pedperei         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_info
{
	int				nbr_philo;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				times_to_eat;
	pthread_t		*threads;
	long int		start;
	pthread_mutex_t	*forks;
	pthread_mutex_t	instruction;
	pthread_mutex_t	eat;
	pthread_mutex_t	die;
}					t_info;

typedef struct s_philo
{
	long int		last_eat;
	long int		time_thinking;
	long int		time_sleeping;
	int				nbr;
	int				nbr_eats;
	int				is_eating;
	int				fork;
	t_info			*info;
}					t_philo;

int					ft_atoi(const char *str);
void				*ft_calloc(size_t nitems, size_t size);
int					init_process(t_philo *philos, t_info *info);

#endif
