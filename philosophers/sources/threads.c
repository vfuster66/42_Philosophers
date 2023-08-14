/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 09:08:17 by vfuster-          #+#    #+#             */
/*   Updated: 2023/08/14 09:14:07 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

// Processus de prise des fourchettes par un philosophe
// Verouille les mutex, enregistre le temps du dernier repas,
// signale que le philosophe mange, deverouille les mutex
void	take_fork(t_philosophers *philosophers)
{
	pthread_mutex_lock
		(&philosophers->table->mutex_forks[philosophers->left_fork]);
	message(philosophers->table, philosophers->philosopher_number,
		"🍴  has taken the left fork");
	pthread_mutex_lock
		(&philosophers->table->mutex_forks[philosophers->right_fork]);
	message(philosophers->table, philosophers->philosopher_number,
		"🍴  has taken the right fork");
	pthread_mutex_lock(&philosophers->mutex_eating);
	philosophers->last_meal_eaten = get_time();
	philosophers->is_eating = 1;
	message(philosophers->table, philosophers->philosopher_number,
		"🍔 is eating");
	ft_usleep(philosophers->table->time_to_eat);
	philosophers->is_eating = 0;
	philosophers->meal_counter++;
	pthread_mutex_unlock(&philosophers->mutex_eating);
}

// Execution de chaque thread de philosophe
void	*routine(void *argument)
{
	t_philosophers	*philosophers;

	philosophers = ((t_philosophers *)argument);
	while (philosophers->table->end)
	{
		take_fork(philosophers);
		pthread_mutex_unlock
			(&philosophers->table->mutex_forks[philosophers->left_fork]);
		pthread_mutex_unlock
			(&philosophers->table->mutex_forks[philosophers->right_fork]);
		message(philosophers->table,
			philosophers->philosopher_number, "💤 is_sleeping");
		ft_usleep(philosophers->table->time_to_sleep);
		message(philosophers->table,
			philosophers->philosopher_number, "💭 is thinking");
	}
	return (NULL);
}

// Demarre la simulation en creant les threads pour chaque
// philosophe ainsi que les moniteurs de surveillance
void	start_simulation(t_arguments *table)
{
	int	i;

	i = 0;
	while (i < table->number_of_philosophers)
	{
		table->philosophers[i]->last_meal_eaten = get_time();
		pthread_create(&table->philosophers[i]->philo_thread, NULL, routine,
			(void *)table->philosophers[i]);
		i++;
		usleep(100);
	}
	i = 0;
	while (i < table->number_of_philosophers)
	{
		pthread_create(&table->philosophers[i]->monitor_thread, NULL, monitor,
			(void *)table->philosophers[i]);
		i++;
		usleep(100);
	}
}
