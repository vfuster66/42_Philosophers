/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checks.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/03 18:25:35 by vfuster-          #+#    #+#             */
/*   Updated: 2023/08/03 18:34:15 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	check_command_line_arguments(int ac, char **av)
{
	int	i;
	int	result;

	if (ac != 5 && ac != 6)
	{
		printf("%s", BHRED ERR_CMD RESET);
	}
	i = 0;
	while (av[++i])
	{
		result = ft_atoi(av[i]);
		if (i == 1 && result > 250)
		{
			printf(BHRED ERR_MAX RESET);
			return (-1);
		}
		if (result <= 0)
		{
			printf(BHRED ERR_ARGS RESET);
			return (-1);
		}
	}
	return (0);
}

void	check_philosopher_death(t_philo *philo)
{
	pthread_mutex_lock(&philo->args->checker_mutex);
	if ((get_timestamp(philo->args) - philo->last_meal_time)
		>= philo->args->time_to_die)
	{
		print_status(philo, BHRED "died          💀 🪦" RESET);
		philo->args->game_over = 1;
	}
	pthread_mutex_unlock(&philo->args->checker_mutex);
}

void	checker(t_global *c)
{
	int	meal_i;
	int	death_i;

	meal_i = 0;
	death_i = 0;
	while (c->game_over == 0)
	{
		check_philosopher_death(&c->philosophers[death_i]);
		if (c->max_meals > 0)
		{
			while (meal_i < c->philosophers_number)
			{
				if (c->philosophers[meal_i].meals_eaten < c->max_meals)
					break ;
				meal_i++;
			}
			if (meal_i == c->philosophers_number)
				c->game_over = 1;
		}
		usleep(5000);
		if (death_i == c->philosophers_number -1)
			death_i = -1;
		death_i++;
	}
}

void	*philosopher_thread(void *args)
{
	t_philo	*philosopher;

	philosopher = (t_philo *)args;
	pthread_mutex_lock(&philosopher->args->forks[philosopher->left_fork]);
	print_status(philosopher, BHMAG "has taken a fork 🍴" RESET);
	philosopher->last_meal_time = get_timestamp(philosopher->args);
	ft_sleep(philosopher, philosopher->args->time_to_die);
	print_status(philosopher, BHRED "died 💀 🪦" RESET);
	philosopher->args->game_over = 1;
	return (NULL);
}
