/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   help.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slimane <slimane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 19:57:55 by slimane           #+#    #+#             */
/*   Updated: 2025/06/28 20:36:13 by slimane          ###   ########.fr       */
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
		usleep(333);
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
