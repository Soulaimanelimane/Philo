/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   second_help.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slimane <slimane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 21:42:21 by slimane           #+#    #+#             */
/*   Updated: 2025/06/29 16:27:55 by slimane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_mutex_destroy(t_philos *data, int i)
{
	int	j;

	j = 0;
	pthread_mutex_destroy(&data->print);
	pthread_mutex_destroy(&data->data_lo);
	pthread_mutex_destroy(&data->done_meal);
	while (j < i)
	{
		pthread_mutex_destroy(&data->fork[j]);
		j++;
	}
	free(data->fork);
	return (-1);
}

int	ft_init_var(t_philos **data, char **av)
{
	(*data)->philo_is_die = 0;
	(*data)->num_philo = ft_atoi(av[1]);
	(*data)->time_to_die = ft_atoi(av[2]);
	(*data)->time_to_eat = ft_atoi(av[3]);
	(*data)->time_to_sleep = ft_atoi(av[4]);
	(*data)->start_time = get_time();
	(*data)->meals_done = 0;
	if (av[5])
		(*data)->num_meals = ft_atoi(av[5]);
	else
		(*data)->num_meals = -1;
	return (0);
}

int	ft_init_data(t_philos *data, char **av)
{
	int	i;

	if (ft_init_var(&data, av) != 0)
		return (-1);
	i = 0;
	data->fork = malloc(sizeof(pthread_mutex_t) * data->num_philo);
	if (data->fork == NULL)
		return (-1);
	if (pthread_mutex_init(&data->print, NULL) != 0)
		return (free(data->fork), -1);
	if (pthread_mutex_init(&data->data_lo, NULL) != 0)
		return (free(data->fork), pthread_mutex_destroy(&data->print), 1);
	if (pthread_mutex_init(&data->done_meal, NULL) != 0)
		return (free(data->fork), pthread_mutex_destroy(&data->print),
			pthread_mutex_destroy(&data->data_lo), -1);
	while (i < data->num_philo)
	{
		if (pthread_mutex_init(&data->fork[i], NULL) != 0)
			return (ft_mutex_destroy(data, i));
		i++;
	}
	return (0);

}

void    ft_check_dead(t_philo *data)
{
	if (data == NULL || data->info == NULL)
		return ;
	lock(&data->info->data_lo, 1);
	data->die = 1;
	data->info->philo_is_die = 1;
	lock(&data->info->data_lo, 2);
	lock(&data->info->print, 1);
	printf("%ld %d died\n",get_time() - data->start, data->id);
	lock(&data->info->print, 2);
	
}

int	ft_check_die(t_philo **data)
{
	int	i;
	int	k;

	i = 0;
	k = 0;
	while(i < (*data)->info->num_philo)
	{
		lock(&(*data)[i].meal, 1);
		if ((get_time() - (*data)[i].last_meal) > (*data)->info->time_to_die)
		{
			lock(&(*data)[i].meal, 2);
			ft_check_dead(&(*data)[i]);
			return (1);
		}
		(lock(&(*data)[i].meal, 2), lock(&(*data)[i].count_m, 1));
		if ((*data)[i].count_meals == (*data)[i].info->num_meals)
			k++;
		lock(&(*data)[i].info->done_meal, 1);
		if (k == (*data)[i].info->num_philo)
		{
			(*data)[i].info->meals_done = 1;
			lock(&(*data)[i].info->done_meal, 2);
			return (lock(&(*data)[i].count_m, 2), 1);
		}
		lock(&(*data)[i].info->done_meal, 2);
		(lock(&(*data)[i].count_m, 2), i++);
	}
	return (0);
}

