/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 09:08:17 by vfuster-          #+#    #+#             */
/*   Updated: 2023/08/14 09:14:07 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

// Processus de prise des fourchettes par un philosophe
// Verouille les mutex, enregistre le temps du dernier repas,
// signale que le philosophe mange, deverouille les mutex
void take_fork(t_philosophers *philosophers)
{
    // Le philosophe verrouille le mutex de la fourchette gauche
    pthread_mutex_lock(&philosophers->table->mutex_forks[philosophers->left_fork]);
    
    // Le philosophe affiche qu'il a pris la fourchette gauche
    message(philosophers->table, philosophers->philosopher_number, BWHT "🍴  has taken the left fork" RESET);

    // Le philosophe verrouille le mutex de la fourchette droite
    pthread_mutex_lock(&philosophers->table->mutex_forks[philosophers->right_fork]);

    // Le philosophe affiche qu'il a pris la fourchette droite
    message(philosophers->table, philosophers->philosopher_number, BWHT "🍴  has taken the right fork" RESET);

    // Le philosophe verrouille le mutex 'mutex_eating'
    pthread_mutex_lock(&philosophers->mutex_eating);

    // Met à jour le temps du dernier repas mangé
    philosophers->last_meal_eaten = get_time();

    // Indique que le philosophe est en train de manger
    philosophers->is_eating = 1;

    // Affiche que le philosophe est en train de manger
    message(philosophers->table, philosophers->philosopher_number, BGRN "🍔  is eating" RESET);

    // Fait dormir le philosophe pendant la durée du repas
    ft_usleep(philosophers->table->time_to_eat);

    // Indique que le philosophe a fini de manger
    philosophers->is_eating = 0;

    // Incrémente le compteur de repas du philosophe
    philosophers->meal_counter++;

    // Déverrouille le mutex 'mutex_eating'
    pthread_mutex_unlock(&philosophers->mutex_eating);
}


// Execution de chaque thread de philosophe
void *routine(void *argument)
{
    t_philosophers *philosophers;

    // Cast le pointeur argument en tant que t_philosophers
    philosophers = ((t_philosophers *)argument);

    // Boucle continue tant que la simulation n'est pas terminée
    while (philosophers->table->end)
    {
        // Le philosophe tente de prendre les fourchettes pour manger
        take_fork(philosophers);

        // Une fois qu'il a mangé, il déverrouille les fourchettes
        pthread_mutex_unlock(&philosophers->table->mutex_forks[philosophers->left_fork]);
        pthread_mutex_unlock(&philosophers->table->mutex_forks[philosophers->right_fork]);

        // Le philosophe affiche qu'il dort
        message(philosophers->table, philosophers->philosopher_number, BBLU "💤  is_sleeping" RESET);

        // Le philosophe dort pendant un certain temps
        ft_usleep(philosophers->table->time_to_sleep);

        // Le philosophe affiche qu'il pense
        message(philosophers->table, philosophers->philosopher_number, BMAG "💭  is thinking" BMAG);
    }

    // Retourne NULL à la fin du thread
    return (NULL);
}


// Demarre la simulation en creant les threads pour chaque
// philosophe ainsi que les moniteurs de surveillance
void start_simulation(t_arguments *table)
{
    int i;

    i = 0;
    // Crée les threads pour les philosophes
    while (i < table->number_of_philosophers)
    {
        // Initialise le temps du dernier repas mangé par le philosophe
        table->philosophers[i]->last_meal_eaten = get_time();

        // Crée un thread pour le philosophe en utilisant la fonction 'routine'
        pthread_create(&table->philosophers[i]->philo_thread, NULL, routine, (void *)table->philosophers[i]);

        // Incrémente 'i' et ajoute une courte pause entre les créations de thread
        i++;
        usleep(100);
    }

    // Réinitialise 'i' pour créer les threads des moniteurs
    i = 0;
    while (i < table->number_of_philosophers)
    {
        // Crée un thread pour le moniteur associé au philosophe
        pthread_create(&table->philosophers[i]->monitor_thread, NULL, monitor, (void *)table->philosophers[i]);

        // Incrémente 'i' et ajoute une courte pause entre les créations de thread
        i++;
        usleep(100);
    }
}
