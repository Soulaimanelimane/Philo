/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slimane <slimane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 13:07:59 by slimane           #+#    #+#             */
/*   Updated: 2025/06/27 16:57:20 by slimane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	get_time(void)
{
	struct timeval	t;

	gettimeofday(&t, NULL);
	return ((t.tv_sec * 1000) + (t.tv_usec / 1000));
	
}

int	lock(pthread_mutex_t *data, int flag)
{
	if (flag == 1)
		pthread_mutex_lock(data);
	else
		pthread_mutex_unlock(data);
	return (0);
}

void	ft_usleep(long t, t_philo *philo)
{
	long	time;

	time = get_time();
	while ((get_time() - time) < t)
	{
		lock(&philo->info->data_lo, 1);
		if (philo->info->philo_is_die == 1)
		{
			lock(&philo->info->data_lo, 2);
			break;
		}
		lock(&philo->info->data_lo, 2);	
		usleep(100);
	}
}

int	check_arg(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] > 57 || str[i] < 48)
			return (1);
		i++;
	}
	return (0);
}

int	parse_arg(char **str)
{
	int	i;

	i = 1;
	while (str[i])
	{
		if (check_arg(str[i]))
		{
			write(2,"INVALID ARGUMENT FOR PHILO\n", 27);
			return (1);
		}
		i++;
	}
	return (0);
}

int	ft_mutex_destroy(t_philos *data, int i)
{
	int	j;

	j = 0;
	pthread_mutex_destroy(&data->print);
	while (j < i)
	{
		pthread_mutex_destroy(&data->fork[j]);
		j++;
	}
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
		return (-1);
	pthread_mutex_init(&data->data_lo, NULL);
	pthread_mutex_init(&data->done_meal, NULL);
	while (i < data->num_philo)
	{
		if (pthread_mutex_init(&data->fork[i], NULL) != 0)
			return (ft_mutex_destroy(data, i));
		i++;
	}
	return 0;

}

void ft_check_dead(t_philo *data)
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
		if (k == (*data)[i].info->num_philo)
		{
			(*data)[i].info->meals_done = 1;
			return (lock(&(*data)[i].count_m, 2), 1);
		}
		lock(&(*data)[i].count_m, 2);
		i++;
	}
	return (0);
}


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
	printf("%ld %d has taken right fork\n", get_time() - philo->start, philo->id);
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
	printf("%ld %d has taken left fork\n", get_time() - philo->start, philo->id);
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
		usleep(10);
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


