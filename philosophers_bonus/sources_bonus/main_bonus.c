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

int main(int ac, char **av)
{
    t_arguments *table;

    // Vérifie la validité des arguments passés en ligne de commande
    if (check_arguments_validity(av, ac))
        return (1);

    // Initialise la structure t_arguments
    table = initialize_arguments(ac, av);

    // Vérifie si l'initialisation a échoué
    if (!table)
        return (1);

    // Enregistre le moment de démarrage de la simulation
    table->time_starter = get_time();

    // Démarre la simulation
    start_simulation(table);

    // La simulation s'exécute indéfiniment, donc cette ligne n'est atteinte que lorsque la simulation se termine
    return (0);
}

