/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checks.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 09:03:33 by vfuster-          #+#    #+#             */
/*   Updated: 2023/08/14 09:05:26 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

// Verifie si le nombre de repas est atteint
void check_meal(t_philosophers *philosopher)
{
    int i;

    // Initialise 'i' à 0 pour parcourir les philosophes et vérifier leurs repas
    i = 0;

    // Verrouille le mutex 'mutex_eating' pour éviter les conflits lors de la vérification
    pthread_mutex_lock(&philosopher->mutex_eating);

    // Boucle 'while' pour vérifier le nombre de repas de chaque philosophe
    while (i < philosopher->table->number_of_meals)
    {
        // Vérifie si le philosophe actuel a atteint le nombre de repas souhaité
        if (philosopher->table->philosophers[i]->meal_counter >= philosopher->table->number_of_meals)
        {
            // Si le dernier philosophe a terminé ses repas, met fin à la simulation
            if (i == philosopher->table->number_of_philosophers - 1)
            {
                // Verrouille le mutex 'mutex_print' pour éviter les conflits lors de l'affichage
                pthread_mutex_lock(&philosopher->table->mutex_print);

                // Modifie la propriété 'end' de la table à 0 pour arrêter la simulation
                philosopher->table->end = 0;

                // Déverrouille le mutex 'mutex_print'
                pthread_mutex_unlock(&philosopher->table->mutex_print);
            }

            // Passe au philosophe suivant
            i++;
        }
        else
        {
            // Si le philosophe actuel n'a pas encore terminé ses repas, sort de la boucle
            break;
        }
    }

    // Déverrouille le mutex 'mutex_eating'
    pthread_mutex_unlock(&philosopher->mutex_eating);
}


// Verifie si un philosophe est en train de manger et si le temps
// max autorise avant le prochain repas est depasse
void *monitor(void *argument)
{
    t_philosophers *philosophers;

    // Cast le pointeur argument en tant que t_philosophers
    philosophers = ((t_philosophers *)argument);

    // Boucle continue tant que la simulation n'est pas terminée
    while (philosophers->table->end)
    {
        // Vérifie si le philosophe n'est pas en train de manger et si le temps écoulé depuis le dernier repas est supérieur au temps limite
        if (!philosophers->is_eating && get_time() - philosophers->last_meal_eaten >= philosophers->table->time_to_die)
        {
            // Verrouille le mutex 'mutex_eating' pour éviter les conflits lors de la modification des propriétés
            pthread_mutex_lock(&philosophers->mutex_eating);

            // Affiche que le philosophe est mort
            message(philosophers->table, philosophers->philosopher_number, BRED "⚰️   died" RESET);

            // Met fin à la simulation en modifiant la propriété 'end' de la table à 0
            philosophers->table->end = 0;

            // Déverrouille le mutex 'mutex_eating'
            pthread_mutex_unlock(&philosophers->mutex_eating);
        }

        // Vérifie si le philosophe a atteint le nombre de repas souhaité
        if (philosophers->table->number_of_meals && philosophers->meal_counter >= philosophers->table->number_of_meals)
            check_meal(philosophers);

        // Fait une pause courte pour éviter une utilisation intensive du processeur
        usleep(100);
    }

    // Retourne NULL à la fin du thread
    return (NULL);
}


// Verifie si une chaine de caracteres est numerique
int str_is_number(char *str)
{
    int i;

    // Initialise 'i' à 0 pour parcourir chaque caractère de la chaîne 'str'
    i = 0;

    // Boucle 'while' pour parcourir chaque caractère de la chaîne 'str'
    while (str[i])
    {
        // Vérifie si le caractère actuel n'est pas un chiffre (0-9)
        if (str[i] < '0' || str[i] > '9')
            return (1); // Renvoie 1 si le caractère n'est pas un chiffre

        // Incrémente 'i' pour passer au caractère suivant
        i++;
    }

    // Si tous les caractères sont des chiffres, la fonction renvoie 0
    return (0);
}


// Verifie la validite des arguments en ligne de commande
int check_arguments_validity(char **av, int ac)
{
    int i;

    // Initialise 'i' à 1, car on commence à vérifier les arguments à partir de l'indice 1
    i = 1;

    // Vérifie si le nombre d'arguments est valide (entre 5 et 6 inclus)
    if (ac < 5 || ac > 6)
    {
        printf("%s%s%s", BRED, ARG_ERROR, RESET);
        return (1);
    }

    // Boucle 'while' pour vérifier chaque argument
    while (i < ac)
    {
        // Vérifie si l'argument commence par '-' ou s'il n'est pas un nombre valide
        if (av[i][0] == '-' || str_is_number(av[i]) || ft_atoi(av[i]) == 0)
        {
            printf("%s%s%s", BRED, INV_ERROR, RESET);
            return (1);
        }

        // Incrémente 'i' pour passer à l'argument suivant
        i++;
    }

    // Si tous les arguments sont valides, la fonction retourne 0
    return (0);
}

