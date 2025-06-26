/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slimane <slimane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 13:07:59 by slimane           #+#    #+#             */
/*   Updated: 2025/06/26 22:46:32 by slimane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
/*
long	get_time(void)
{
	struct timeval	t;

	gettimeofday(&t, NULL);
	return ((t.tv_sec * 1000) + (t.tv_usec / 1000));
	
}



void	ft_usleep(long t, t_philo *philo)
{
	long	time;

	time = get_time();
	while ((get_time() - time) < t)
	{
		pthread_mutex_lock(&philo->info->data_lo);
		if (philo->info->philo_is_die == 1)
		{
			pthread_mutex_unlock(&philo->info->data_lo);
			break;
		}
		pthread_mutex_unlock(&philo->info->data_lo);	
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

int	ft_init_data(t_philos *data, char **av)
{
	int	i;

	i = 0;
	data->philo_is_die = 0;
	data->num_philo = ft_atoi(av[1]);
	data->time_to_die = ft_atoi(av[2]);
	data->time_to_eat = ft_atoi(av[3]);
	data->time_to_sleep = ft_atoi(av[4]);
	data->start_time = get_time();
	data->meals_done = 0;
	if (av[5])
		data->num_meals = ft_atoi(av[5]);
	else
		data->num_meals = -1;
	data->fork = malloc(sizeof(pthread_mutex_t) * data->num_philo);
	if (data->fork == NULL)
		return (-1);
	if (pthread_mutex_init(&data->print, NULL) != 0)
		return (-1);
	if (pthread_mutex_init(&data->data_lo, NULL) != 0)
		return (pthread_mutex_destroy(&data->print), -1);
	if (pthread_mutex_init(&data->done_meal, NULL) != 0)
		return (pthread_mutex_destroy(&data->print), pthread_mutex_destroy(&data->data_lo), -1);
	while (i < data->num_philo)
	{
		if (pthread_mutex_init(&data->fork[i], NULL) != 0)
			return (ft_mutex_destroy(data, i));
		i++;
	}
	return (0);

}

int	check_nm(t_philo philo)
{
	pthread_mutex_lock(&philo.meal);
	long i = philo.last_meal;
	pthread_mutex_unlock(&philo.meal);
	if (get_time() - i > philo.info->time_to_die)
		return (2);
	if (philo.count_meals == philo.info->num_meals)
		return (1);
	return (0);
}

// int	check_philos(t_philo *data)
// {

// }

void	moniteur(t_philo *data)
{
	int	k;
	int	i;
	while (1)
	{
		pthread_mutex_lock(&data[0].info->data_lo);
		if (data->info->philo_is_die == 1)
		{
			pthread_mutex_unlock(&data[0].info->data_lo);
			return ;
		}
		pthread_mutex_unlock(&data[0].info->data_lo);
		k = 0;
		i = 0;
		while(i < data->info->num_philo)
		{
			pthread_mutex_lock(&data[i].meal);
			if ((get_time() - data[i].last_meal) > data->info->time_to_die)
			{
				pthread_mutex_unlock(&data[i].meal);
				pthread_mutex_lock(&data[i].info->data_lo);
				data[i].die = 1;
				data[i].info->philo_is_die = 1;
				pthread_mutex_unlock(&data[i].info->data_lo);
				pthread_mutex_lock(&data[i].info->print);
				printf("%ld %d died :(\n",(get_time() - data[i].start), data[i].id);
				pthread_mutex_unlock(&data[i].info->print);
				return ;
			}
			pthread_mutex_unlock(&data[i].meal);
			pthread_mutex_lock(&data[i].count_m);
			if (data[i].count_meals >= data[i].info->num_meals)
				k++;
			if (k >= data[i].info->num_philo)
			{
				data[i].info->meals_done = 1;
				pthread_mutex_unlock(&data[i].count_m);
				return ;
			}
			pthread_mutex_unlock(&data[i].count_m);

			i++;
		}
		//check_philos(data);
	}
}




int	should_exit(t_philo *philo)
{

	pthread_mutex_lock(&philo->info->data_lo);
	if (philo->info->philo_is_die || philo->info->meals_done)
		return (pthread_mutex_unlock(&philo->info->data_lo), 1);
	return (pthread_mutex_unlock(&philo->info->data_lo), 0);
}

void	*routine(void *data)
{
	t_philo *philo = (t_philo *)data;

	pthread_mutex_lock(&philo->meal);
	philo->last_meal = philo->start;
	pthread_mutex_unlock(&philo->meal);

	if (philo->id % 2 == 0)
		ft_usleep(philo->info->time_to_eat / 2, philo);

	while (1)
	{
		if (should_exit(philo))
			break;

		pthread_mutex_lock(philo->right_fork);
		pthread_mutex_lock(&philo->info->print);
		if (should_exit(philo))
		{
			pthread_mutex_unlock(&philo->info->print);
			pthread_mutex_unlock(philo->right_fork);
			break;
		}
		printf("%ld %d has taken right fork\n", get_time() - philo->info->start_time, philo->id);
		pthread_mutex_unlock(&philo->info->print);

		if (should_exit(philo))
		{
			pthread_mutex_unlock(philo->right_fork);
			break;
		}

		pthread_mutex_lock(philo->left_fork);
		pthread_mutex_lock(&philo->info->print);
		if (should_exit(philo))
		{
			pthread_mutex_unlock(&philo->info->print);
			pthread_mutex_unlock(philo->left_fork);
			pthread_mutex_unlock(philo->right_fork);
			break;
		}
		printf("%ld %d has taken left fork\n", get_time() - philo->info->start_time, philo->id);
		if (should_exit(philo))
		{
			pthread_mutex_unlock(&philo->info->print);
			pthread_mutex_unlock(philo->left_fork);
			pthread_mutex_unlock(philo->right_fork);
			break;
		}
		pthread_mutex_unlock(&philo->info->print);

		pthread_mutex_lock(&philo->info->print);
		if (should_exit(philo))
		{
			pthread_mutex_unlock(&philo->info->print);
			pthread_mutex_unlock(philo->left_fork);
			pthread_mutex_unlock(philo->right_fork);
			break;
		}
		printf("%ld %d is eating\n", get_time() - philo->info->start_time, philo->id);
		pthread_mutex_unlock(&philo->info->print);

		
		ft_usleep(philo->info->time_to_eat, philo);

		pthread_mutex_lock(&philo->meal);
		philo->last_meal = get_time();
		pthread_mutex_unlock(&philo->meal);
		pthread_mutex_lock(&philo->count_m);
		philo->count_meals++;
		pthread_mutex_unlock(&philo->count_m);
		if (should_exit(philo))
		{
			pthread_mutex_unlock(philo->left_fork);
			pthread_mutex_unlock(philo->right_fork);
			break;
		}


		if (should_exit(philo))
		{
			pthread_mutex_unlock(philo->left_fork);
			pthread_mutex_unlock(philo->right_fork);
			break;
		}

		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);

		pthread_mutex_lock(&philo->info->print);
		if (should_exit(philo))
		{
			pthread_mutex_unlock(&philo->info->print);
			break;
		}
		printf("%ld %d is sleeping\n", get_time() - philo->info->start_time, philo->id);
		pthread_mutex_unlock(&philo->info->print);

		ft_usleep(philo->info->time_to_sleep, philo);

		if (should_exit(philo))
			break;

		pthread_mutex_lock(&philo->info->print);
		if (should_exit(philo))
		{
			pthread_mutex_unlock(&philo->info->print);
			break;
		}
		printf("%ld %d is thinking\n", get_time() - philo->info->start_time, philo->id);
		pthread_mutex_unlock(&philo->info->print);

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
		// usleep(50);
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

*/



long	get_time(void)
{
	struct timeval	t;

	gettimeofday(&t, NULL);
	return ((t.tv_sec * 1000) + (t.tv_usec / 1000));
	
}

void	ft_usleep(long t, t_philo *philo)
{
	long	time;

	time = get_time();
	while ((get_time() - time) < t)
	{
		pthread_mutex_lock(&philo->info->data_lo);
		if (philo->info->philo_is_die == 1)
		{
			pthread_mutex_unlock(&philo->info->data_lo);
			break;
		}
		pthread_mutex_unlock(&philo->info->data_lo);	
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

int	ft_init_data(t_philos *data, char **av)
{
	int	i;

	i = 0;
	data->philo_is_die = 0;
	data->num_philo = ft_atoi(av[1]);
	data->time_to_die = ft_atoi(av[2]);
	data->time_to_eat = ft_atoi(av[3]);
	data->time_to_sleep = ft_atoi(av[4]);
	data->start_time = get_time();
	data->meals_done = 0;
	if (av[5])
		data->num_meals = ft_atoi(av[5]);
	else
		data->num_meals = -1;
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

int	check_nm(t_philo philo)
{
	pthread_mutex_lock(&philo.meal);
	long i = philo.last_meal;
	pthread_mutex_unlock(&philo.meal);
	if (get_time() - i > philo.info->time_to_die)
		return (2);
	if (philo.count_meals == philo.info->num_meals)
		return (1);
	return (0);
}

void	moniteur(t_philo *data)
{
	int i = 0;
	int k = 0;
	while (1)
	{
		pthread_mutex_lock(&data[0].info->data_lo);
		if (data->info->philo_is_die == 1)
		{
			pthread_mutex_unlock(&data[0].info->data_lo);
			return ;
		}
		pthread_mutex_unlock(&data[0].info->data_lo);
		i = 0;
		k = 0;
		while(i < data->info->num_philo)
		{
			pthread_mutex_lock(&data[i].meal);
			if ((get_time() - data[i].last_meal) > data->info->time_to_die)
			{
				pthread_mutex_unlock(&data[i].meal);
				pthread_mutex_lock(&data[i].info->data_lo);
				data[i].die = 1;
				data->info->philo_is_die = 1;
				pthread_mutex_unlock(&data[i].info->data_lo);
				pthread_mutex_lock(&data[i].info->print);
				printf("%ld %d died :(\n",(get_time() - data[i].start), data[i].id);
				pthread_mutex_unlock(&data[i].info->print);
				return ;
			}
			pthread_mutex_unlock(&data[i].meal);
			
			pthread_mutex_lock(&data[i].count_m);
			if (data[i].count_meals == data[i].info->num_meals)
				k++;
			if (k == data[i].info->num_philo)
			{
				// pthread_mutex_lock(&data[i].ini)
				data[i].info->meals_done = 1;
				pthread_mutex_unlock(&data[i].count_m);
				return ;
			}
			pthread_mutex_unlock(&data[i].count_m);

			i++;
		}
		// i = 0;
	}
}




int	should_exit(t_philo *philo)
{

	pthread_mutex_lock(&philo->info->data_lo);
	if (philo->info->philo_is_die || philo->info->meals_done)
		return (pthread_mutex_unlock(&philo->info->data_lo), 1);
	return (pthread_mutex_unlock(&philo->info->data_lo), 0);
}

void	*routine(void *data)
{
	t_philo *philo = (t_philo *)data;

	pthread_mutex_lock(&philo->meal);
	philo->last_meal = philo->start;
	pthread_mutex_unlock(&philo->meal);

	if (philo->id % 2 == 0)
		ft_usleep(philo->info->time_to_eat / 2, philo);

	while (1)
	{
		if (should_exit(philo))
			break;

		pthread_mutex_lock(philo->right_fork);
		pthread_mutex_lock(&philo->info->print);
		if (should_exit(philo))
		{
			pthread_mutex_unlock(&philo->info->print);
			pthread_mutex_unlock(philo->right_fork);
			break;
		}
		printf("%ld %d has taken right fork\n", get_time() - philo->info->start_time, philo->id);
		pthread_mutex_unlock(&philo->info->print);

		if (should_exit(philo))
		{
			pthread_mutex_unlock(philo->right_fork);
			break;
		}

		pthread_mutex_lock(philo->left_fork);
		pthread_mutex_lock(&philo->info->print);
		if (should_exit(philo))
		{
			pthread_mutex_unlock(&philo->info->print);
			pthread_mutex_unlock(philo->left_fork);
			pthread_mutex_unlock(philo->right_fork);
			break;
		}
		printf("%ld %d has taken left fork\n", get_time() - philo->info->start_time, philo->id);
		if (should_exit(philo))
		{
			pthread_mutex_unlock(&philo->info->print);
			pthread_mutex_unlock(philo->left_fork);
			pthread_mutex_unlock(philo->right_fork);
			break;
		}
		pthread_mutex_unlock(&philo->info->print);

		pthread_mutex_lock(&philo->info->print);
		if (should_exit(philo))
		{
			pthread_mutex_unlock(&philo->info->print);
			pthread_mutex_unlock(philo->left_fork);
			pthread_mutex_unlock(philo->right_fork);
			break;
		}
		printf("%ld %d is eating\n", get_time() - philo->info->start_time, philo->id);
		pthread_mutex_unlock(&philo->info->print);

		ft_usleep(philo->info->time_to_eat, philo);

		if (should_exit(philo))
		{
			pthread_mutex_unlock(philo->left_fork);
			pthread_mutex_unlock(philo->right_fork);
			break;
		}

		pthread_mutex_lock(&philo->meal);
		philo->last_meal = get_time();
		pthread_mutex_unlock(&philo->meal);
		pthread_mutex_lock(&philo->count_m);
		philo->count_meals++;
		pthread_mutex_unlock(&philo->count_m);

		if (should_exit(philo))
		{
			pthread_mutex_unlock(philo->left_fork);
			pthread_mutex_unlock(philo->right_fork);
			break;
		}

		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);

		pthread_mutex_lock(&philo->info->print);
		if (should_exit(philo))
		{
			pthread_mutex_unlock(&philo->info->print);
			break;
		}
		printf("%ld %d is sleeping\n", get_time() - philo->info->start_time, philo->id);
		pthread_mutex_unlock(&philo->info->print);

		ft_usleep(philo->info->time_to_sleep, philo);

		if (should_exit(philo))
			break;

		pthread_mutex_lock(&philo->info->print);
		if (should_exit(philo))
		{
			pthread_mutex_unlock(&philo->info->print);
			break;
		}
		printf("%ld %d is thinking\n", get_time() - philo->info->start_time, philo->id);
		pthread_mutex_unlock(&philo->info->print);

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
		// usleep(50);
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


