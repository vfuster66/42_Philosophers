/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 09:09:12 by vfuster-          #+#    #+#             */
/*   Updated: 2023/08/14 14:26:11 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes_bonus/philosophers_bonus.h"

// Initialise les philosophes, alloue de la memoire pour les structures
// initialise les mutex associes et configurent leurs proprietes
t_philosophers	**initialize_philosophers(t_arguments *table)
{
	t_philosophers	**philosophers;
	int				i;

	philosophers = (t_philosophers **)malloc(sizeof(t_philosophers *)
			* table->number_of_philosophers);
	if (!philosophers)
		return (NULL);
	i = 0;
	while (i < (int)table->number_of_philosophers)
	{
		philosophers[i] = (t_philosophers *)malloc(sizeof(t_philosophers) * 1);
		if (!philosophers[i])
			return (0);
		philosophers[i]->philosopher_number = i;
		philosophers[i]->table = table;
		sem_unlink("eatcounter");
		philosophers[i]->sem_eat = sem_open("eatcounter", O_CREAT, 0644, 1);
		if (philosophers[i]->sem_eat == SEM_FAILED)
			return (NULL);
		i++;
	}
	return (philosophers);
}

// Initialise la structure principale en utilisant
// les arguments en ligne de commande
// Cree les philosophes et les fourchettes
t_arguments	*initialize_arguments(int ac, char **av)
{
	t_arguments	*table;

	table = malloc(sizeof(t_arguments) * 1);
	if (!table)
		return (NULL);
	table->number_of_philosophers = (__uint64_t)ft_atoi(av[1]);
	table->time_to_die = (__uint64_t)ft_atoi(av[2]);
	table->time_to_eat = (__uint64_t)ft_atoi(av[3]);
	table->time_to_sleep = (__uint64_t)ft_atoi(av[4]);
	if (table->number_of_philosophers == -1 || table->time_to_die == -1
		|| table->time_to_eat == -1 || table->time_to_sleep == -1
		|| table->number_of_meals == -1)
	{
		printf("%s%s%s", BRED, INV_ERROR, RESET);
		return (NULL);
	}
	table->number_of_meals = 0;
	if (ac == 6)
		table->number_of_meals = ft_atoi(av[5]);
	if (initialize_semaphores(table))
		return (NULL);
	table->time_starter = get_time();
	table->philosophers = initialize_philosophers(table);
	return (table);
}

int	initialize_semaphores(t_arguments *table)
{
	sem_unlink("forking");
	table->sem_forks = sem_open("forking", O_CREAT, 0644,
			table->number_of_philosophers);
	if (table->sem_forks == SEM_FAILED)
		return (1);
	sem_unlink("printing");
	table->sem_print = sem_open("printing", O_CREAT, 0644, 1);
	if (table->sem_print == SEM_FAILED)
		return (1);
	sem_unlink("meal");
	table->sem_meal = sem_open("meal", O_CREAT, 0644, 1);
	if (table->sem_meal == SEM_FAILED)
		return (1);
	return (0);
}
