/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/03 18:24:26 by vfuster-          #+#    #+#             */
/*   Updated: 2023/08/03 18:33:31 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	philosopher_eat(t_philo *philosopher)
{
	if (philosopher->args->game_over == 0)
	{
		pthread_mutex_lock(&philosopher->args->forks[philosopher->left_fork]);
		print_status(philosopher, BHMAG "has taken a fork 🍴" RESET);
		pthread_mutex_destroy
			(&philosopher->args->forks[philosopher->right_fork]);
		print_status(philosopher, BHMAG "has taken a fork 🍴" RESET);
		print_status(philosopher, BHYEL "is eating     😋 🍝" RESET);
		philosopher->last_meal_time = get_timestamp(philosopher->args);
		philosopher->meals_eaten += 1;
		ft_sleep(philosopher, philosopher->args->time_to_eat);
		pthread_mutex_unlock(&philosopher->args->forks[philosopher->left_fork]);
		pthread_mutex_unlock
			(&philosopher->args->forks[philosopher->right_fork]);
	}
}

void	philosopher_sleep(t_philo *philosopher)
{
	if (philosopher->args->game_over == 0)
		print_status(philosopher, BHCYN "is sleeping   😴 💤" RESET);
	ft_sleep(philosopher, philosopher->args->time_to_sleep);
}

void	philosopher_think(t_philo *philosopher)
{
	if (philosopher->args->game_over == 0)
		print_status(philosopher, BHGRN "is thinking   🤔 💭" RESET);
}

void	*philosopher_actions(void *argument)
{
	t_philo		*philosopher;
	t_global	*input;

	philosopher = (t_philo *)argument;
	input = philosopher->args;
	if (philosopher->id % 2)
		usleep(1500);
	while (input->game_over == 0)
	{
		if (input->max_meals > 0 
			&& philosopher->meals_eaten == input->max_meals)
			break ;
		philosopher_eat(philosopher);
		philosopher_sleep(philosopher);
		philosopher_think(philosopher);
	}
	return (NULL);
}
