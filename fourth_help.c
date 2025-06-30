/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fourth_help.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slimane <slimane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 12:01:53 by slimane           #+#    #+#             */
/*   Updated: 2025/06/30 12:28:30 by slimane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_handel_one(t_philo *philo)
{
	if (should_exit(philo))
		return (1);
	if (philo->info->num_philo == 1)
	{
		lock(philo->right_fork, 1);
		lock(&philo->info->print, 1);
		printf("%ld %d has taken a fork\n", get_time() - philo->start,
			philo->id);
		return (lock(philo->right_fork, 2), lock(&philo->info->print, 2), 1);
	}
	return (0);
}

void	ft_set_last_meal(t_philo *philo)
{
	lock(&philo->meal, 1);
	philo->last_meal = philo->start;
	lock(&philo->meal, 2);
}

int	init_philos(t_philo *philos, t_philos *data)
{
	int	i;

	i = 0;
	while (i < data->num_philo)
	{
		ft_init_philo_var(&philos[i], data, i);
		if (pthread_mutex_init(&philos[i].meal, NULL) != 0)
		{
			ft_destroy_philo(philos, 0, i);
			return (-1);
		}
		if (pthread_mutex_init(&philos[i].count_m, NULL) != 0)
		{
			ft_destroy_philo(philos, 1, i);
			return (-1);
		}
		i++;
	}
	return (0);
}
