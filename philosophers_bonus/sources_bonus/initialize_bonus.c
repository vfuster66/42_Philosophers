/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 09:09:12 by vfuster-          #+#    #+#             */
/*   Updated: 2023/08/14 14:26:11 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes_bonus/philosophers_bonus.h"

// Initialise les philosophes, alloue de la memoire pour les structures
// initialise les mutex associes et configurent leurs proprietes
t_philosophers **initialize_philosophers(t_arguments *table)
{
    t_philosophers **philosophers;
    int i;

    // Alloue de la mémoire pour le tableau de pointeurs de philosophes en fonction du nombre de philosophes dans la table
    philosophers = (t_philosophers **)malloc(sizeof(t_philosophers *) * table->number_of_philosophers);
    if (!philosophers)
        return (NULL);

    // Initialise un compteur i à 0
    i = 0;
    while (i < (int)table->number_of_philosophers)
    {
        // Alloue de la mémoire pour chaque philosophe individuel
        philosophers[i] = (t_philosophers *)malloc(sizeof(t_philosophers) * 1);
        if (!philosophers[i])
            return (0);

        // Affecte le numéro du philosophe
        philosophers[i]->philosopher_number = i;

        // Affecte le pointeur vers la table partagée
        philosophers[i]->table = table;

        // Supprime un sémaphore nommé "eatcounter" s'il existe déjà
        sem_unlink("eatcounter");

        // Crée un sémaphore nommé "eatcounter" avec une valeur initiale de 1 (permis unique)
        philosophers[i]->sem_eat = sem_open("eatcounter", O_CREAT, 0644, 1);
        if (philosophers[i]->sem_eat == SEM_FAILED)
            return (NULL);

        // Incrémente le compteur i pour passer au philosophe suivant
        i++;
    }

    // Retourne le tableau de pointeurs de philosophes
    return (philosophers);
}

// Initialise la structure principale en utilisant
// les arguments en ligne de commande
// Cree les philosophes et les fourchettes
t_arguments *initialize_arguments(int ac, char **av)
{
    t_arguments *table;

    // Alloue de la mémoire pour la structure t_arguments
    table = malloc(sizeof(t_arguments) * 1);
    if (!table)
        return (NULL);

    // Affecte les valeurs des arguments aux champs de la structure table
    table->number_of_philosophers = (__uint64_t)ft_atoi(av[1]);
    table->time_to_die = (__uint64_t)ft_atoi(av[2]);
    table->time_to_eat = (__uint64_t)ft_atoi(av[3]);
    table->time_to_sleep = (__uint64_t)ft_atoi(av[4]);

    // Vérifie si les valeurs des champs sont valides, affiche une erreur et retourne NULL en cas d'erreur
    if (table->number_of_philosophers == -1 || table->time_to_die == -1
        || table->time_to_eat == -1 || table->time_to_sleep == -1
        || table->number_of_meals == -1)
    {
        printf("%s%s%s", BRED, INV_ERROR, RESET);
        return (NULL);
    }

    // Initialise le champ number_of_meals à 0
    table->number_of_meals = 0;

    // Si un sixième argument est fourni, affecte sa valeur à number_of_meals
    if (ac == 6)
        table->number_of_meals = ft_atoi(av[5]);

    // Appelle la fonction initialize_semaphores pour initialiser les sémaphores
    if (initialize_semaphores(table))
        return (NULL);

    // Initialise le champ time_starter avec le temps actuel
    table->time_starter = get_time();

    // Appelle la fonction initialize_philosophers pour initialiser les philosophes
    table->philosophers = initialize_philosophers(table);

    // Retourne la structure table
    return (table);
}

int initialize_semaphores(t_arguments *table)
{
    // Supprime tout sémaphore existant nommé "forking"
    sem_unlink("forking");
    // Initialise le sémaphore sem_forks avec le nombre de fourchettes disponibles
    table->sem_forks = sem_open("forking", O_CREAT, 0644, table->number_of_philosophers);
    // Vérifie si l'initialisation du sémaphore sem_forks a échoué
    if (table->sem_forks == SEM_FAILED)
        return (1);

    // Supprime tout sémaphore existant nommé "printing"
    sem_unlink("printing");
    // Initialise le sémaphore sem_print avec 1 pour contrôler l'accès à l'affichage
    table->sem_print = sem_open("printing", O_CREAT, 0644, 1);
    // Vérifie si l'initialisation du sémaphore sem_print a échoué
    if (table->sem_print == SEM_FAILED)
        return (1);

    // Supprime tout sémaphore existant nommé "meal"
    sem_unlink("meal");
    // Initialise le sémaphore sem_meal avec 1 pour contrôler les repas des philosophes
    table->sem_meal = sem_open("meal", O_CREAT, 0644, 1);
    // Vérifie si l'initialisation du sémaphore sem_meal a échoué
    if (table->sem_meal == SEM_FAILED)
        return (1);

    // Retourne 0 pour indiquer que l'initialisation des sémaphores s'est déroulée sans erreur
    return (0);
}
