/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/19 14:52:53 by vfuster-          #+#    #+#             */
/*   Updated: 2023/08/07 15:01:42 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <limits.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <stdbool.h>

// Colors

//Regular text
# define BLK "\e[0;30m"
# define RED "\e[0;31m"
# define GRN "\e[0;32m"
# define YEL "\e[0;33m"
# define BLU "\e[0;34m"
# define MAG "\e[0;35m"
# define CYN "\e[0;36m"
# define WHT "\e[0;37m"

//Regular bold text
# define BBLK "\e[1;30m"
# define BRED "\e[1;31m"
# define BGRN "\e[1;32m"
# define BYEL "\e[1;33m"
# define BBLU "\e[1;34m"
# define BMAG "\e[1;35m"
# define BCYN "\e[1;36m"
# define BWHT "\e[1;37m"

//Regular underline text
# define UBLK "\e[4;30m"
# define URED "\e[4;31m"
# define UGRN "\e[4;32m"
# define UYEL "\e[4;33m"
# define UBLU "\e[4;34m"
# define UMAG "\e[4;35m"
# define UCYN "\e[4;36m"
# define UWHT "\e[4;37m"

//Regular background
# define BLKB "\e[40m"
# define REDB "\e[41m"
# define GRNB "\e[42m"
# define YELB "\e[43m"
# define BLUB "\e[44m"
# define MAGB "\e[45m"
# define CYNB "\e[46m"
# define WHTB "\e[47m"

//High intensty background
# define BLKHB "\e[0;100m"
# define REDHB "\e[0;101m"
# define GRNHB "\e[0;102m"
# define YELHB "\e[0;103m"
# define BLUHB "\e[0;104m"
# define MAGHB "\e[0;105m"
# define CYNHB "\e[0;106m"
# define WHTHB "\e[0;107m"

//High intensty text
# define HBLK "\e[0;90m"
# define HRED "\e[0;91m"
# define HGRN "\e[0;92m"
# define HYEL "\e[0;93m"
# define HBLU "\e[0;94m"
# define HMAG "\e[0;95m"
# define HCYN "\e[0;96m"
# define HWHT "\e[0;97m"

//Bold high intensity text
# define BHBLK "\e[1;90m"
# define BHRED "\e[1;91m"
# define BHGRN "\e[1;92m"
# define BHYEL "\e[1;93m"
# define BHBLU "\e[1;94m"
# define BHMAG "\e[1;95m"
# define BHCYN "\e[1;96m"
# define BHWHT "\e[1;97m"

//Reset
# define RESET "\e[0m"

// Macros
# define MAX_PHILOS      250
# define STR_MAX_PHILOS  "250"

// Erreurs
# define ERR_CMD "❌ Erreur dans la ligne de commande.\n" 
# define ERR_MAX "❌ Nombre de philosophes invalide. Maximum : 250\n" 
# define ERR_ARGS "❌ Les arguments doivent être des entiers \
	compris entre  0 et 2147483647.\n"  

typedef struct s_control	t_control;
typedef struct s_philo		t_philo;

// Structures
typedef struct s_global
{
	int					philosophers_number;
	int					max_meals;
	int					end_meal;
	int					error;
	int					game_over;
	long long			start_time;
	long long			time_to_die;
	long long			time_to_eat;
	long long			time_to_sleep;
	t_philo				*philosophers;
	pthread_mutex_t		*forks;
	pthread_mutex_t		print_mutex;
	pthread_mutex_t		checker_mutex;
}	t_global;

typedef struct s_philo
{
	pthread_t			thread_id;
	int					id;
	int					meals_eaten;
	int					left_fork;
	int					right_fork;
	long long			last_meal_time;
	t_global			*args;
}	t_philo;

// Fonctions
int			check_command_line_arguments(int ac, char **av);
void		initialize_global_settings(char **av, t_global *input);
void		initialize_philosophers(t_global *input);

long long	get_start_time(void);
long long	get_timestamp(t_global *input);
void		ft_sleep(t_philo *philo, int time_to_wait);

int			ft_atoi(char const *str);
void		ft_bzero(void *s, size_t n);
void		*ft_calloc(size_t count, size_t size);
void		print_status(t_philo *philo, char *state);

void		*philosopher_thread(void *philo);
void		start_simulation(t_global *input);
void		*philosopher_actions(void *philo);
void		end_simulation(t_global *input);
void		checker(t_global *input);
void		check_philosopher_death(t_philo *philo);

void		philosopher_eat(t_philo *philo);
void		philosopher_think(t_philo *philo);
void		philosopher_sleep(t_philo *philo);

#endif
