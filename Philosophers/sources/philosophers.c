/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/03 12:29:16 by vfuster-          #+#    #+#             */
/*   Updated: 2023/08/07 15:25:06 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	start_simulation(t_global *input)
{
	int	i;

	if (input->philosophers_number == 1)
	{
		philosopher_thread(input->philosophers);
	}
	else
	{
		i = -1;
		while (++i < input->philosophers_number)
			pthread_create(&input->philosophers[i].thread_id, NULL,
				&philosopher_actions, (void *) &input->philosophers[i]);
		while (input->game_over == 0)
			checker(input);
	}
}

void	initialize_philosophers(t_global *input)
{
	int	i;

	i = 0;
	while (i < input->philosophers_number)
	{
		input->philosophers[i].id = i + 1;
		input->philosophers[i].meals_eaten = 0;
		input->philosophers[i].last_meal_time = 0;
		input->philosophers[i].left_fork = i;
		input->philosophers[i].right_fork
			= (i + 1) % input->philosophers_number;
		input->philosophers[i].args = input;
		i++;
	}
}

void    initialize_global_settings(char **av, t_global *input)
{
        int     i;

        i = -1;
        input->error = 0;
        input->game_over = 0;
        input->end_meal = 0;
	if (!av[1] || !av[2] || !av[3] || !av[4])
	{
		input->error = 1;
		return ;
	}
        input->philosophers_number = ft_atoi(av[1]);
        input->time_to_die = ft_atoi(av[2]);
        input->time_to_eat = ft_atoi(av[3]);
        input->time_to_sleep = ft_atoi(av[4]);
	if (input->philosophers_number <= 0
			|| input->time_to_die <= 0
			|| input->time_to_eat <= 0
			|| input->time_to_sleep <= 0)
	{
		input->error = 1;
		return ;
	}
        if (av[5])
                input->max_meals = ft_atoi(av[5]);
        else
                input->max_meals = 0;
        input->philosophers
                = ft_calloc(input->philosophers_number, (sizeof(t_philo)));
        input->forks
                = ft_calloc(input->philosophers_number, (sizeof(pthread_mutex_t)));
        pthread_mutex_init(&input->print_mutex, NULL);
        pthread_mutex_init(&input->checker_mutex, NULL);
        while (++i < input->philosophers_number)
                pthread_mutex_init(&input->forks[i], NULL);
        initialize_philosophers(input);
        input->start_time = get_start_time();
}

void	end_simulation(t_global *input)
{
	int	i;

	i = input->philosophers_number;
	while (--i >= 0)
		pthread_join(input->philosophers[i].thread_id, NULL);
	while (++i < input->philosophers_number)
		pthread_mutex_destroy(&input->forks[i]);
	pthread_mutex_destroy(&input->print_mutex);
	pthread_mutex_destroy(&input->checker_mutex);
	free(input->philosophers);
	free(input->forks);
	free(input);
}

int	main(int ac, char **av)
{
	t_global	*data;

	if (check_command_line_arguments(ac, av) != 0)
		return (-1);
	data = (t_global *)ft_calloc(1, sizeof(t_global));
	if (!data)
		return (-1);
	initialize_global_settings(av, data);
	start_simulation(data);
	end_simulation(data);
	return (0);
}
