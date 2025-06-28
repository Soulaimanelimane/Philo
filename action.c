/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slimane <slimane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 20:22:01 by slimane           #+#    #+#             */
/*   Updated: 2025/06/28 20:43:54 by slimane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	should_exit(t_philo *philo)
{

	lock(&philo->info->data_lo, 1);
	if (philo->info->philo_is_die || philo->info->meals_done)
		return (lock(&philo->info->data_lo, 2), 1);
	return (lock(&philo->info->data_lo, 2), 0);
}

void	take_fork(t_philo *philo)
{
	lock(philo->right_fork, 1), lock(&philo->info->print, 1);
	if (should_exit(philo))
	{
		(lock(&philo->info->print, 2), lock(philo->right_fork, 2));
		return ;
	}
	printf("%ld %d has taken a fork\n", get_time() - philo->start, philo->id);
	lock(&philo->info->print, 2);
	if (should_exit(philo))
		return ((void)lock(philo->right_fork, 2));
	(lock(philo->left_fork, 1), lock(&philo->info->print, 1));
	if (should_exit(philo))
	{
		(lock(&philo->info->print, 2), lock(philo->left_fork, 2));
		lock(philo->right_fork, 2);
		return ;
	}
	printf("%ld %d has taken a fork\n", get_time() - philo->start, philo->id);
	if (should_exit(philo))
	{
		lock(&philo->info->print, 2), lock(philo->left_fork, 2);
		lock(philo->right_fork, 2);
		return ;
	}
	lock(&philo->info->print, 2);
}

void	ft_eat(t_philo	*philo)
{
		lock(&philo->info->print, 1);
		if (should_exit(philo))
		{
			(lock(&philo->info->print, 2), lock(philo->left_fork, 2));
			lock(philo->right_fork, 2);
			return ;
		}
		printf("%ld %d is eating\n", get_time() - philo->start, philo->id);
		lock(&philo->info->print, 2);
		ft_usleep(philo->info->time_to_eat, philo);
		if (should_exit(philo))
		{
			(lock(philo->left_fork, 2), lock(philo->right_fork, 2));
			return ;
		}
		lock(&philo->meal, 1);
		philo->last_meal = get_time();
		lock(&philo->meal, 2);
		lock(&philo->count_m, 1);
		philo->count_meals++;
		lock(&philo->count_m, 2);
		if (should_exit(philo))
			return (lock(philo->left_fork, 2), (void)lock(philo->right_fork, 2));
		(lock(philo->left_fork, 2), lock(philo->right_fork, 2));
}

void	ft_sleep(t_philo *philo)
{
	lock(&philo->info->print, 1);
	if (should_exit(philo))
	{
		lock(&philo->info->print, 2);
		return ;
	}
	printf("%ld %d is sleeping\n", get_time() - philo->start, philo->id);
	lock(&philo->info->print, 2);
	ft_usleep(philo->info->time_to_sleep, philo);
}

void	ft_think(t_philo *philo)
{
	lock(&philo->info->print, 1);
	if (should_exit(philo))
	{
		lock(&philo->info->print, 2);
		return ;
	}
	printf("%ld %d is thinking\n", get_time() - philo->info->start_time, philo->id);
	lock(&philo->info->print, 2);
}
