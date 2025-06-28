/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slimane <slimane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 13:07:59 by slimane           #+#    #+#             */
/*   Updated: 2025/06/28 21:43:05 by slimane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"


void	moniteur(t_philo *data)
{

	while (1)
	{
		usleep(100);
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
	t_philo *philo;

	philo = (t_philo *)data;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         
	lock(&philo->meal, 1);
	(philo->last_meal = philo->start, lock(&philo->meal, 2));
	if (philo->id % 2 == 0)
		ft_usleep(philo->info->time_to_eat / 2, philo);
	while (1)
	{
		if (should_exit(philo))
			return (NULL);
		take_fork(philo);
		if (should_exit(philo))
			break;
		ft_eat(philo);
		if (should_exit(philo))
			break;
		ft_sleep(philo);
		if (should_exit(philo))
			break;
		ft_think(philo);
		if (should_exit(philo))
			break;
	}
	return (NULL);
}

int main(int ac, char **av)
{
	t_philos	data;
	t_philo		*philos;
	int	i = 0;
	
	if (ac == 5 || ac == 6)
	{
		if (parse_arg(av) == 1)
			return (1);
		if (ft_init_data(&data, av) == -1)
			return (1);

		philos = malloc(sizeof(t_philo) * data.num_philo);
		while(i < data.num_philo)
		{
			philos[i].id = i + 1;
			philos[i].info = &data;
			philos[i].last_meal = 0;
			philos[i].count_meals = 0;
			philos[i].die = 0;
			philos[i].start = get_time();
			philos[i].right_fork = &data.fork[i];
			philos[i].left_fork = &data.fork[(i + 1) % data.num_philo];
			i++;
		}
		i = 0;
		while(i < data.num_philo)
		{
			pthread_mutex_init(&philos[i].meal, NULL);
			pthread_mutex_init(&philos[i].count_m, NULL);
			pthread_create(&philos[i].thread, NULL, routine, &philos[i]);
			i++;
		}
		//usleep(10);
		moniteur(philos);
		i = 0;
		while (i < data.num_philo)
		{
			pthread_join(philos[i].thread, NULL);
			i++;
		}
		return (0);
	}
	write(2, "ERROR ARGGUMENTS ARE EMPTY\n", 27);
	return (1);
}


