/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   processus_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 11:45:02 by vfuster-          #+#    #+#             */
/*   Updated: 2023/08/14 14:26:49 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes_bonus/philosophers_bonus.h"

// Processus de prise des fourchettes par un philosophe
// Verouille les mutex, enregistre le temps du dernier repas,
// signale que le philosophe mange, deverouille les mutex
void take_fork(t_philosophers *philosophers)
{
    // Attend la disponibilité des fourchettes en utilisant un sémaphore
    sem_wait(philosophers->table->sem_forks);
    // Affiche un message indiquant que le philosophe a pris la fourchette gauche
    message(philosophers->table, philosophers->philosopher_number,
            BWHT "🍴  has taken the left fork" RESET);
    
    // Attend la disponibilité des fourchettes en utilisant un autre sémaphore
    sem_wait(philosophers->table->sem_forks);
    // Affiche un message indiquant que le philosophe a pris la fourchette droite
    message(philosophers->table, philosophers->philosopher_number,
            BWHT "🍴  has taken the right fork" RESET);
    
    // Affiche un message indiquant que le philosophe est en train de manger
    message(philosophers->table, philosophers->philosopher_number,
            BGRN "🍔  is eating" RESET);
    
    // Attente mutuelle pour empêcher plusieurs philosophes de manger en même temps
    sem_wait(philosophers->sem_eat);
    
    // Signale qu'un repas a eu lieu en incrémentant un sémaphore
    sem_post(philosophers->table->sem_meal);
    
    // Enregistre l'heure à laquelle le philosophe a commencé à manger
    philosophers->last_meal_eaten = get_time();
    
    // Attente pour simuler le temps que le philosophe passe à manger
    ft_usleep(philosophers->table->time_to_eat);
    
    // Libère le sémaphore pour permettre à d'autres philosophes de manger
    sem_post(philosophers->sem_eat);
}


// Execution de chaque thread de philosophe
void *routine(t_philosophers *philosophers)
{
    while (1)
    {
        // Le philosophe essaie de prendre les fourchettes et commence à manger
        take_fork(philosophers);

        // Libère les fourchettes après avoir mangé
        sem_post(philosophers->table->sem_forks);
        sem_post(philosophers->table->sem_forks);

        // Affiche un message indiquant que le philosophe est en train de dormir
        message(philosophers->table,
                philosophers->philosopher_number, BBLU "💤  is_sleeping" RESET);

        // Attente pour simuler le temps que le philosophe passe à dormir
        ft_usleep(philosophers->table->time_to_sleep);

        // Affiche un message indiquant que le philosophe est en train de réfléchir
        message(philosophers->table,
                philosophers->philosopher_number, BMAG "💭  is thinking" RESET);

        // Petite attente pour éviter une utilisation excessive du processeur
        usleep(100);
    }
    return (NULL);
}


// Demarre la simulation en creant les threads pour chaque
// philosophe ainsi que les moniteurs de surveillance
void start_simulation(t_arguments *table)
{
    int i;

    i = 0;

    // Si le nombre de repas est spécifié, lance le moniteur de repas
    if (table->number_of_meals)
        pthread_create(&table->meal_monitor, NULL,
            monitor_meal, (void *)table);

    while (i < table->number_of_philosophers)
    {
        // Crée un processus fils pour chaque philosophe
        table->philosophers[i]->philosopher_pid = fork();
        table->philosophers[i]->last_meal_eaten = get_time();

        // Si c'est le processus fils
        if (table->philosophers[i]->philosopher_pid == 0)
        {
            // Crée un thread pour surveiller la mort du philosophe
            pthread_create(&table->philosophers[i]->monitor_die,
                NULL, monitor_die,
                (void *)table->philosophers[i]);

            // Appelle la fonction routine pour simuler le comportement du philosophe
            routine(table->philosophers[i]);

            // Termine le processus fils
            exit(1);
        }

        i++;
        usleep(100);
    }

    // Ferme les processus fils
    close_processes(table);
}


void close_processus(t_arguments *table)
{
    int i;
    int status;

    i = 0;
    waitpid(-1, &status, 0); // Attend qu'un processus fils se termine

    if (WIFEXITED(status) || WIFSIGNALED(status))
    {
        // Si le processus fils s'est terminé normalement ou à cause d'un signal
        while (i < table->number_of_philosophers)
        {
            // Tue tous les processus fils associés aux philosophes en utilisant SIGKILL
            kill(table->philosophers[i++]->philosopher_pid, SIGKILL);
        }
    }
}

