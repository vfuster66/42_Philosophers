/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 09:06:04 by vfuster-          #+#    #+#             */
/*   Updated: 2023/08/14 14:07:19 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes_bonus/philosophers_bonus.h"

// Conversion d'une chaine de caracteres en un entier
int	ft_atoi(char *str)
{
	int	i;
	int	result;
	int	sign;

	i = 0;
	result = 0;
	sign = 1;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		str++;
	if (str[i] == '-')
		sign = -1;
	if (str[i] == '+' || str[i] == '-')
		str++;
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + str[i] - '0';
		str++;
	}
	return (result * sign);
}

// Affiche les messages de statuts des philosophes et
// des evenements de simulation
void	message(t_arguments *table, int philo_number, char *message)
{
	sem_wait(table->sem_print);
	printf(BWHT "[%d]\tPhilosopher %d %s\n" RESET, get_time() - table->time_starter,
		philo_number + 1, message);
	if (message[0] != 'd')
		sem_post(table->sem_print);
}

// Met le thread en pause
void	ft_usleep(__uint64_t time_in_milliseconde)
{
	__uint64_t	time_start;

	time_start = 0;
	time_start = get_time();
	while ((get_time() - time_start) < time_in_milliseconde)
		usleep(time_in_milliseconde / 10);
}

// Renvoie le temps ecoule en ms depuis un moment de reference
int	get_time(void)
{
	static struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * (__uint64_t)1000 + (tv.tv_usec / 1000));
}
