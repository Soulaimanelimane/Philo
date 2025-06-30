/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slimane <slimane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 13:07:59 by slimane           #+#    #+#             */
/*   Updated: 2025/06/30 12:30:07 by slimane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	moniteur(t_philo *data)
{
	while (1)
	{
		lock(&data[0].info->data_lo, 1);
		if (data->info->philo_is_die == 1)
		{
			lock(&data[0].info->data_lo, 2);
			return ;
		}
		lock(&data[0].info->data_lo, 2);
		if (ft_check_die(&data) == 1)
			return ;
	}
}

void	*routine(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	ft_set_last_meal(philo);
	if (philo->id % 2 == 0)
		ft_usleep(philo->info->time_to_eat / 2, philo);
	while (1)
	{
		if (ft_handel_one(philo) == 1)
			break ;
		take_fork(philo);
		if (should_exit(philo))
			break ;
		ft_eat(philo);
		if (should_exit(philo))
			break ;
		ft_sleep(philo);
		if (should_exit(philo))
			break ;
		ft_think(philo);
		if (should_exit(philo))
			break ;
	}
	return (NULL);
}

int	create_threads(t_philo *philos, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		if (pthread_create(&philos[i].thread, NULL, routine, &philos[i]) != 0)
		{
			while (i >= 1)
			{
				i--;
				pthread_detach(philos[i].thread);
			}
			ft_destroy_philo(philos, 1, n);
			return (-1);
		}
		i++;
	}
	return (0);
}

int	start_simulation(t_philos *data, char **av)
{
	t_philo	*philos;
	int		i;

	if (ft_init_data(data, av) == -1)
		return (1);
	philos = malloc(sizeof(t_philo) * data->num_philo);
	if (!philos)
		return (1);
	if (init_philos(philos, data) == -1)
		return (free(philos), 1);
	if (create_threads(philos, data->num_philo) == -1)
		return (free(philos), 1);
	usleep(10);
	moniteur(philos);
	i = 0;
	while (i < data->num_philo)
	{
		pthread_join(philos[i].thread, NULL);
		i++;
	}
	return (free(philos->info->fork), free(philos), 0);
}

int	main(int ac, char **av)
{
	t_philos	data;

	if (ac == 5 || ac == 6)
	{
		if (parse_arg(av) == 1)
			return (1);
		if (start_simulation(&data, av) == 1)
			return (1);
		return (0);
	}
	write(2, "ERROR ARGGUMENTS ARE EMPTY\n", 27);
	return (1);
}
