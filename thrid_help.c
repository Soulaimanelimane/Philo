/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thrid_help.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slimane <slimane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 20:40:58 by slimane           #+#    #+#             */
/*   Updated: 2025/06/30 15:52:54 by slimane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_init_philo_var(t_philo *philo, t_philos *data, int i)
{
	philo->id = i + 1;
	philo->info = data;
	philo->last_meal = 0;
	philo->count_meals = 0;
	philo->die = 0;
	philo->start = get_time();
	philo->right_fork = &data->fork[i];
	philo->left_fork = &data->fork[(i + 1) % data->num_philo];
}

void	ft_destroy_philo(t_philo *philo, int flag, int i)
{
	int	j;

	j = 0;
	while (j < i)
	{
		pthread_mutex_destroy(&philo[j].meal);
		if (flag == 1)
			pthread_mutex_destroy(&philo[i].count_m);
		j++;
	}
	ft_mutex_destroy(philo->info, philo->info->num_philo);
}
