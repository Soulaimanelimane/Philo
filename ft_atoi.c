/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slimane <slimane@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 21:17:44 by slimane           #+#    #+#             */
/*   Updated: 2025/06/22 21:29:14 by slimane          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	overflow(int sign)
{
	if (sign == 1)
		return (-1);
	else
		return (0);
}

int	ft_atoi(const char *str)
{
	 long long		result;
	int						sign;
	 long long		max;
	int						i;

	result = 0;
	sign = 1;
	i = 0;
	max = 9223372036854775807;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] >= 48 && str[i] <= 57)
	{
		if (result > (max - (str[i] - 48)) / 10)
			return (overflow(sign));
		result = result * 10 + (str[i] - 48);
		i++;
	}
	return (result * sign);
}
