/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 08:49:53 by vfuster-          #+#    #+#             */
/*   Updated: 2023/08/14 10:55:55 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes_bonus/philosophers_bonus.h"

int	main(int ac, char **av)
{
	t_arguments	*table;

	if (check_arguments_validity(av, ac))
		return (1);
	table = initialize_arguments(ac, av);
	if (!table)
		return (1);
	table->time_starter = get_time();
	start_simulation(table);
	return (0);
}
