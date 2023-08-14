/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checks_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 09:03:33 by vfuster-          #+#    #+#             */
/*   Updated: 2023/08/14 14:27:32 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes_bonus/philosophers_bonus.h"

// Verifie si le nombre de repas est atteint
void *monitor_meal(void *arguments)
{
    t_arguments *table;
    int meal_counter;
    int i;

    // Convertit l'argument générique en un pointeur vers la structure t_arguments
    table = (t_arguments *)arguments;

    // Initialise le compteur de repas à 0
    meal_counter = 0;

    // Boucle principale pour surveiller le nombre de repas
    while (meal_counter < table->number_of_meals)
    {
        i = 0;

        // Boucle pour attendre que tous les philosophes aient terminé un repas
        while (i <= table->number_of_philosophers)
        {
            // Attend la disponibilité d'un jeton dans le sémaphore 'sem_meal'
            sem_wait(table->sem_meal);
            i++;
        }

        // Incrémente le compteur de repas
        meal_counter++;
    }

    // Attendez le sémaphore 'sem_print' pour verrouiller l'affichage
    sem_wait(table->sem_print);

    i = 0;
    // Boucle pour tuer tous les processus philosophes en envoyant un signal SIGKILL
    while (i < table->number_of_philosophers)
    {
        // Envoie un signal SIGKILL pour tuer le processus du philosophe
        kill(table->philosophers[i++]->philosopher_pid, SIGKILL);
    }

    return (NULL);
}


// Verifie si un philosophe est en train de manger et si le temps
// max autorise avant le prochain repas est depasse
void *monitor_die(void *argument)
{
    t_philosophers *philosophers;

    // Convertit l'argument générique en un pointeur vers la structure t_philosophers
    philosophers = ((t_philosophers *)argument);

    // Boucle infinie pour surveiller la mort des philosophes
    while (1)
    {
        // Attend que le sémaphore 'sem_eat' soit disponible (jeton disponible)
        sem_wait(philosophers->sem_eat);

        // Vérifie si le temps écoulé depuis le dernier repas dépasse le temps limite pour mourir
        if (get_time() - philosophers->last_meal_eaten >= philosophers->table->time_to_die)
        {
            // Affiche un message indiquant que le philosophe est mort et utilise BRED et RESET pour mettre en évidence le message
            message(philosophers->table, philosophers->philosopher_number, BRED "⚰️   died" RESET);

            // Termine le processus du philosophe en utilisant la fonction exit
            exit(1);
        }

        // Libère le sémaphore 'sem_eat'
        sem_post(philosophers->sem_eat);

        // Pause courte pour éviter une utilisation excessive du processeur
        usleep(100);
    }

    return (NULL);
}


// Verifie si une chaine de caracteres est numerique
int str_is_number(char *str)
{
    int i;

    // Initialise un compteur i à 0
    i = 0;

    // Boucle pour parcourir chaque caractère de la chaîne str
    while (str[i])
    {
        // Vérifie si le caractère n'est pas un chiffre (0-9)
        if (str[i] < '0' || str[i] > '9')
        {
            // Si un caractère n'est pas un chiffre, retourne 1 (vrai) indiquant que la chaîne n'est pas composée que de chiffres
            return (1);
        }
        // Incrémente le compteur i pour passer au caractère suivant
        i++;
    }

    // Si tous les caractères sont des chiffres, retourne 0 (faux) indiquant que la chaîne est composée uniquement de chiffres
    return (0);
}


// Verifie la validite des arguments en ligne de commande
int check_arguments_validity(char **av, int ac)
{
    int i;

    // Initialise un compteur i à 1 (car les arguments commencent à partir de l'indice 1)
    i = 1;

    // Vérifie le nombre d'arguments (ac) et s'assure qu'il est entre 5 et 6 inclus
    if (ac < 5 || ac > 6)
    {
        // Affiche un message d'erreur en utilisant BRED, ARG_ERROR et RESET pour mettre en évidence le message
        printf("%s%s%s", BRED, ARG_ERROR, RESET);

        // Retourne 1 (vrai) pour indiquer que les arguments ne sont pas valides
        return (1);
    }

    // Boucle pour vérifier chaque argument (à partir de l'indice 1)
    while (i < ac)
    {
        // Vérifie si le premier caractère de l'argument est '-' ou si l'argument n'est pas un nombre valide
        if (av[i][0] == '-' || str_is_number(av[i]) || ft_atoi(av[i]) == 0)
        {
            // Affiche un message d'erreur en utilisant BRED, INV_ERROR et RESET pour mettre en évidence le message
            printf("%s%s%s", BRED, INV_ERROR, RESET);

            // Retourne 1 (vrai) pour indiquer que les arguments ne sont pas valides
            return (1);
        }

        // Incrémente le compteur i pour passer à l'argument suivant
        i++;
    }

    // Si toutes les vérifications passent, retourne 0 (faux) indiquant que les arguments sont valides
    return (0);
}

