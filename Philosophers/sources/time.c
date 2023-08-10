/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/03 18:28:44 by vfuster-          #+#    #+#             */
/*   Updated: 2023/08/03 18:36:45 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

long long	get_timestamp(t_global *input)
{
	struct timeval	current;

	gettimeofday(&current, NULL);
	return (((current.tv_sec * 1000)
			+ (current.tv_usec / 1000)) - input->start_time);
}

void	ft_sleep(t_philo *philosopher, int time_to_wait)
{
	long long	current_time;
	long long	diff;

	current_time = get_timestamp(philosopher->args);
	while (1)
	{
		diff = (get_timestamp(philosopher->args) - current_time);
		if (diff >= time_to_wait)
			break ;
		usleep(1000);
	}
}

long long	get_start_time(void)
{
	struct timeval	start_time;

	gettimeofday(&start_time, NULL);
	return ((start_time.tv_sec * 1000) + (start_time.tv_usec / 1000));
}
