/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_bonus.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/14 08:32:56 by vfuster-          #+#    #+#             */
/*   Updated: 2023/08/14 16:12:19 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_BONUS_H
# define PHILOSOPHERS_BONUS_H

# include <stdio.h>
# include <pthread.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <semaphore.h>
# include <signal.h>
# include <fcntl.h>
# include <sys/wait.h>

// Structure Philosophe
typedef struct s_philosophers
{
	pid_t				philosopher_pid;
	pthread_t			monitor_die;
	struct s_arguments	*table;
	int					philosopher_number;
	int					last_meal_eaten;
	sem_t				*sem_eat;
}						t_philosophers;

// Structure arguments passes au programme
typedef struct s_arguments
{
	pthread_t			meal_monitor;
	int					number_of_philosophers;
	int					time_to_die;
	int					time_to_eat;
	int					time_to_sleep;
	int					number_of_meals;
	int					time_starter;
	sem_t				*sem_print;
	sem_t				*sem_meal;
	sem_t				*sem_forks;
	t_philosophers		**philosophers;
}						t_arguments;

// Couleurs
# define BBLK "\e[1;30m"
# define BRED "\e[1;31m"
# define BGRN "\e[1;32m"
# define BYEL "\e[1;33m"
# define BBLU "\e[1;34m"
# define BMAG "\e[1;35m"
# define BCYN "\e[1;36m"
# define BWHT "\e[1;37m"
# define RESET "\e[0m"

// Erreurs
# define ARG_ERROR "⚠️  Error : wrong argument number \n"
# define INV_ERROR "⚠️  Error : invalid arguments\n"

// Fonctions

/*<-- utils_bonus.c -->*/
int				ft_atoi(char *str);
int				get_time(void);
void			message(t_arguments *table, int philo_number, char *msg);
void			ft_usleep(__uint64_t time_in_milliseconde);

/*<-- initialize_bonus.c -->*/
t_arguments		*initialize_arguments(int ac, char **av);
t_philosophers	**initialize_philosophers(t_arguments *table);
int				initialize_semaphores(t_arguments *table);

/*<-- checks_bonus.c -->*/
int				str_is_number(char *string);
int				check_arguments_validity(char **av, int ac);
void			*monitor_meal(void *arguments);
void			*monitor_die(void *argument);

/*<-- processus_bonus -->*/
void			take_fork(t_philosophers *philosophers);
void			*routine(t_philosophers *philosophers);
void			start_simulation(t_arguments *table);
void			close_processus(t_arguments *table);

#endif
