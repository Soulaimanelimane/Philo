/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slimane <slimane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 20:12:24 by slimane           #+#    #+#             */
/*   Updated: 2025/06/26 20:10:26 by slimane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
#define PHILO_H

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

typedef struct s_philos
{
	int				num_philo;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	long			start_time;
	int				num_meals;
	int				philo_is_die;
	int				meals_done;
	pthread_mutex_t data_lo;
	pthread_mutex_t	print;
	pthread_mutex_t	*fork;
	pthread_mutex_t	done_meal;
}					t_philos;

typedef struct s_philo
{
	int				id;
	long			last_meal;
	long			count_meals;
	int				die;
	long			start;
	t_philos		*info;
	pthread_t		thread;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t meal;
	pthread_mutex_t	count_m;
}					t_philo;

int	ft_atoi(const char *str);

#endif
