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

int main(int ac, char **av)
{
    t_arguments *table;

    // Vérifie la validité des arguments en ligne de commande
    if (check_arguments_validity(av, ac))
        return (1);
	
    // Initialise la structure de la simulation avec les arguments fournis
    table = initialize(ac, av);
    
    // Vérifie si l'initialisation s'est déroulée avec succès
    if (!table)
        return (1);
    
    // Initialise la variable 'end' à 1 pour indiquer que la simulation démarre
    table->end = 1;
    
    // Récupère le temps de démarrage de la simulation
    table->time_starter = get_time();
    
    // Démarre la simulation en créant les threads pour les philosophes et les moniteurs
    start_simulation(table);
    
    // Attente jusqu'à ce que la simulation se termine (lorsque 'end' devient 0)
    while (table->end)
        continue;

    // Retourne 0 pour indiquer que le programme s'est terminé avec succès
    return (0);
}

