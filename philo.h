/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slimane <slimane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 20:12:24 by slimane           #+#    #+#             */
/*   Updated: 2025/07/01 23:41:50 by slimane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>

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
	pthread_mutex_t	data_lo;
	pthread_mutex_t	print;
	pthread_mutex_t	*fork;
	pthread_mutex_t	done_meal;
}					t_philos;

typedef struct s_philo
{
	int				id;
	long			last_meal;
	long			count_meals;
	long			start;
	t_philos		*info;
	pthread_t		thread;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	meal;
	pthread_mutex_t	count_m;
}					t_philo;

int		ft_atoi(const char *str);
long	get_time(void);
int		lock(pthread_mutex_t *data, int flag);
void	ft_usleep(long t, t_philo *philo);
int		check_arg(char *str);
int		parse_arg(char **str);
int		should_exit(t_philo *philo);
void	*take_fork(t_philo *philo);
void	ft_eat(t_philo	*philo);
void	ft_sleep(t_philo *philo);
void	ft_think(t_philo *philo);
int		ft_check_die(t_philo **data);
void	ft_check_dead(t_philo *data);
int		ft_init_data(t_philos *data, char **av);
int		ft_init_var(t_philos **data, char **av);
int		ft_mutex_destroy(t_philos *data, int i);
void	ft_init_philo_var(t_philo *philo, t_philos *data, int i);
void	ft_destroy_philo(t_philo *philo, int flag, int i);
int		ft_handel_one(t_philo *philo);
void	ft_set_last_meal(t_philo *philo);
int		init_philos(t_philo *philos, t_philos *data);

#endif
