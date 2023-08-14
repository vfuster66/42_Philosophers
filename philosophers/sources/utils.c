/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 09:06:04 by vfuster-          #+#    #+#             */
/*   Updated: 2023/08/14 09:07:25 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

// Conversion d'une chaine de caracteres en un entier
int	ft_atoi(char *str)
{
	int	i;
	int	result;
	int	sign;

	i = 0;
	result = 0;
	sign = 1;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		str++;
	if (str[i] == '-')
		sign = -1;
	if (str[i] == '+' || str[i] == '-')
		str++;
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + str[i] - '0';
		str++;
	}
	return (result * sign);
}

// Affiche les messages de statuts des philosophes et
// des evenements de simulation
void message(t_arguments *table, int philo_number, char *message)
{
    // Verrouille le mutex 'mutex_print' pour éviter les conflits lors de l'affichage
    pthread_mutex_lock(&table->mutex_print);

    // Affiche le message formaté avec des informations sur le temps, le numéro du philosophe et le message
    printf("[%d]\tPhilosopher %d %s\n", get_time() - table->time_starter, philo_number + 1, message);

    // Vérifie si le message n'est pas un message de décès
    if (message[0] != 'd')
    {
        // Déverrouille le mutex 'mutex_print'
        pthread_mutex_unlock(&table->mutex_print);
    }
}

// Met le thread en pause
void ft_usleep(__uint64_t time_in_millisecond)
{
    __uint64_t time_start;

    // Initialise la variable 'time_start' à 0
    time_start = 0;

    // Obtenir le temps de départ en utilisant la fonction 'get_time'
    time_start = get_time();

    // Boucle tant que le temps écoulé depuis le temps de départ est inférieur au temps spécifié
    while ((get_time() - time_start) < time_in_millisecond)
    {
        // Fait une pause courte en utilisant la fonction 'usleep'
        usleep(time_in_millisecond / 10);
    }
}

// Renvoie le temps ecoule en ms depuis un moment de reference
int get_time(void)
{
    // Déclare une structure timeval 'tv' pour stocker le temps actuel
    static struct timeval tv;

    // Obtenir le temps actuel avec la fonction gettimeofday
    gettimeofday(&tv, NULL);

    // Calculer le temps en millisecondes et le retourner
    return (tv.tv_sec * (__uint64_t)1000 + (tv.tv_usec / 1000));
}

