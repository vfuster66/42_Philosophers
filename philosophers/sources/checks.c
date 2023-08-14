/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checks.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 09:03:33 by vfuster-          #+#    #+#             */
/*   Updated: 2023/08/14 09:05:26 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

// Verifie si le nombre de repas est atteint
void	check_meal(t_philosophers *philosopher)
{
	int	i;

	i = 0;
	pthread_mutex_lock(&philosopher->mutex_eating);
	while (i < philosopher->table->number_of_meals)
	{
		if (philosopher->table->philosophers[i]->meal_counter
			>= philosopher->table->number_of_meals)
		{
			if (i == philosopher->table->number_of_philosophers - 1)
			{
				pthread_mutex_lock(&philosopher->table->mutex_print);
				philosopher->table->end = 0;
			}
			i++;
		}
		else
			break ;
	}
	pthread_mutex_unlock(&philosopher->mutex_eating);
}

// Verifie si un philosophe est en train de manger et si le temps
// max autorise avant le prochain repas est depasse
void	*monitor(void *argument)
{
	t_philosophers	*philosophers;

	philosophers = ((t_philosophers *)argument);
	while (philosophers->table->end)
	{
		if (!philosophers->is_eating
			&& get_time() - philosophers->last_meal_eaten
			>= philosophers->table->time_to_die)
		{
			pthread_mutex_lock(&philosophers->mutex_eating);
			message(philosophers->table,
				philosophers->philosopher_number, "⚰️  died");
			philosophers->table->end = 0;
			pthread_mutex_unlock(&philosophers->mutex_eating);
		}
		if (philosophers->table->number_of_meals
			&& philosophers->meal_counter
			>= philosophers->table->number_of_meals)
			check_meal(philosophers);
		usleep(100);
	}
	return (NULL);
}

// Verifie si une chaine de caracteres est numerique
int	str_is_number(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (1);
		i++;
	}
	return (0);
}

// Verifie la validite des arguments en ligne de commande
int	check_arguments_validity(char **av, int ac)
{
	int	i;

	i = 1;
	if (ac < 5 || ac > 6)
	{
		printf("%s%s%s", BRED, ARG_ERROR, RESET);
		return (1);
	}
	while (i < ac)
	{
		if (av[i][0] == '-' || str_is_number(av[i])
			|| ft_atoi(av[i]) == 0)
		{
			printf("%s%s%s", BRED, INV_ERROR, RESET);
			return (1);
		}
		i++;
	}
	return (0);
}
