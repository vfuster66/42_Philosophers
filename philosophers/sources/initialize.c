/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 09:09:12 by vfuster-          #+#    #+#             */
/*   Updated: 2023/08/14 09:09:56 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

// Initialise les philosophes, alloue de la memoire pour les structures
// initialise les mutex associes et configurent leurs proprietes
t_philosophers	**initialize_philosophers(t_arguments *table)
{
	t_philosophers	**philosophers;
	int				i;

	philosophers = (t_philosophers **)malloc(sizeof(t_philosophers) * 1);
	if (!philosophers)
		return (NULL);
	i = 0;
	while (i < (int)table->number_of_philosophers)
	{
		philosophers[i] = (t_philosophers *)malloc(sizeof(t_philosophers) * 1);
		if (!philosophers[i])
			return (NULL);
		if (pthread_mutex_init(&philosophers[i]->mutex_eating, NULL))
			return (NULL);
		philosophers[i]->philosopher_number = i;
		philosophers[i]->table = table;
		philosophers[i]->is_eating = 0;
		philosophers[i]->meal_counter = 0;
		philosophers[i]->left_fork = i;
		philosophers[i]->right_fork = (i + 1) % table->number_of_philosophers;
		i++;
	}
	return (philosophers);
}

// Initialise les mutex des fourchettes, alloue de la memoire
pthread_mutex_t	*initialize_forks(t_arguments *table)
{
	pthread_mutex_t	*mutex_forks;
	int				i;

	i = 0;
	if (table->number_of_philosophers == -1 || table->time_to_die == -1
		|| table->time_to_eat == -1 || table->time_to_sleep == -1
		|| table->number_of_meals == -1)
	{
		printf("%s", INV_ERROR);
		return (NULL);
	}
	mutex_forks = malloc(sizeof(pthread_mutex_t)
			* table->number_of_philosophers);
	if (!mutex_forks)
		return (NULL);
	while (i < table->number_of_philosophers)
	{
		pthread_mutex_init(&mutex_forks[i], NULL);
		i++;
	}
	return (mutex_forks);
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
	table->number_of_meals = 0;
	if (ac == 6)
		table->number_of_meals = ft_atoi(av[5]);
	pthread_mutex_init(&table->mutex_print, NULL);
	table->time_starter = get_time();
	table->mutex_forks = initialize_forks(table);
	table->philosophers = initialize_philosophers(table);
	if (!table->philosophers || !table->mutex_forks)
		return (NULL);
	return (table);
}
