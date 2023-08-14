/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checks_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 09:03:33 by vfuster-          #+#    #+#             */
/*   Updated: 2023/08/14 14:05:05 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes_bonus/philosophers_bonus.h"

// Verifie si le nombre de repas est atteint
void	*monitor_meal(void *arguments)
{
	t_arguments	*table;
	int	meal_counter;
	int	i;

	table = (t_arguments *)arguments;
	meal_counter = 0;
	while (meal_counter < table->number_of_meals)
	{
		i = 0;
		while (i <= table->number_of_philosophers)
		{
			sem_wait(table->sem_meal);
			i++;
		}
		meal_counter++;
	}
	sem_wait(table->sem_print);
	i = 0;
	while (i < table->number_of_philosophers)
		kill(table->philosophers[i++]->philosopher_pid, SIGKILL);
	return (NULL);
}

// Verifie si un philosophe est en train de manger et si le temps
// max autorise avant le prochain repas est depasse
void	*monitor_die(void *argument)
{
	t_philosophers	*philosophers;

	philosophers = ((t_philosophers *)argument);
	while (1)
	{
		sem_wait(philosophers->sem_eat);
		if (get_time() - philosophers->last_meal_eaten
			>= philosophers->table->time_to_die)
		{
			message(philosophers->table,
				philosophers->philosopher_number, BRED "⚰️   died" RESET);
			exit(1);
		}
		sem_post(philosophers->sem_eat);
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
