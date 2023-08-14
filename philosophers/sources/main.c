/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 08:49:53 by vfuster-          #+#    #+#             */
/*   Updated: 2023/08/14 09:09:52 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

int	main(int ac, char **av)
{
	t_arguments	*table;

	if (check_arguments_validity(av, ac))
		return (1);
	table = initialize_arguments(ac, av);
	if (!table)
		return (1);
	table->end = 1;
	table->time_starter = get_time();
	start_simulation(table);
	while (table->end)
		continue ;
	return (0);
}
