/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processus_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 11:45:02 by vfuster-          #+#    #+#             */
/*   Updated: 2023/08/14 14:06:56 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes_bonus/philosophers_bonus.h"

// Processus de prise des fourchettes par un philosophe
// Verouille les mutex, enregistre le temps du dernier repas,
// signale que le philosophe mange, deverouille les mutex
void	take_fork(t_philosophers *philosophers)
{
	sem_wait(philosophers->table->sem_forks);
	message(philosophers->table, philosophers->philosopher_number,
		BWHT "🍴  has taken the left fork" RESET);
	sem_wait(philosophers->table->sem_forks);
	message(philosophers->table, philosophers->philosopher_number,
		BWHT "🍴  has taken the right fork" RESET);
	message(philosophers->table, philosophers->philosopher_number,
		BGRN "🍔  is eating" RESET);
	sem_wait(philosophers->sem_eat);
	sem_post(philosophers->table->sem_meal);
	philosophers->last_meal_eaten = get_time();
	ft_usleep(philosophers->table->time_to_eat);
	sem_post(philosophers->sem_eat);
}

// Execution de chaque thread de philosophe
void	*routine(t_philosophers *philosophers)
{
	while (1)
	{
		take_fork(philosophers);
		sem_post(philosophers->table->sem_forks);
		sem_post(philosophers->table->sem_forks);
		message(philosophers->table,
			philosophers->philosopher_number, BBLU "💤  is_sleeping" RESET);
		ft_usleep(philosophers->table->time_to_sleep);
		message(philosophers->table,
			philosophers->philosopher_number, BMAG "💭  is thinking" RESET);
		usleep(100);
	}
	return (NULL);
}

// Demarre la simulation en creant les threads pour chaque
// philosophe ainsi que les moniteurs de surveillance
void	start_simulation(t_arguments *table)
{
	int	i;

	i = 0;
	if (table->number_of_meals)
		pthread_create(&table->meal_monitor, NULL,
			monitor_meal, (void *)table);
	while (i < table->number_of_philosophers)
	{
		table->philosophers[i]->philosopher_pid = fork();
		table->philosophers[i]->last_meal_eaten = get_time();
		if (table->philosophers[i]->philosopher_pid == 0)
		{
			pthread_create(&table->philosophers[i]->monitor_die, NULL, monitor_die,
				(void *)table->philosophers[i]);
			routine(table->philosophers[i]);
			exit(1);
		}
		i++;
		usleep(100);
	}
	close_processus(table);
}

void	close_processus(t_arguments *table)
{
	int	i;
	int	status;

	i = 0;
	waitpid(-1, &status, 0);
	if (WIFEXITED(status) || WIFSIGNALED(status))
	{
		while (i < table->number_of_philosophers)
			kill(table->philosophers[i++]->philosopher_pid, SIGKILL);
	}
}

