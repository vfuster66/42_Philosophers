#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdio.h>
# include <pthread.h>
# include <stdlib.h>
# include <unistd.h>
# include <limits.h>
# include <sys/time.h>

// Structures
typedef struct	s_philosophers
{
	pthread_t	philo_thread;
	pthread_t	monitor_thread;
	int			philosopher_number;
	int			meal_counter;
	int			last_meal_eaten;
	int			is_eating;
	int			left_fork;
	int			right_fork;
	pthread_mutex_t	mutex_eating;
	struct s_arguments	*table;
}				t_philosophers;

typedef struct s_arguments
{
	int			number_of_philosophers;
	int			time_to_die;
	int			time_to_eat;
	int			time_to_sleep;
	int			number_of_meals;
	int			time_starter;
	int			end;
	pthread_mutex_t	mutex_print;
	pthread_mutex_t	*mutex_forks;
	t_philosophers	** philosophers;
}				t_arguments;

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
int			ft_atoi(char *str);
void	check_meal(t_philosophers *philosophers);
void	*monitor(void *argument);
void	message(t_arguments *table, int philo_number, char *message);
void	ft_usleep(__uint64_t);
void	take_fork(t_philosophers *philosophers);
void	*routine(void *argument);
void	start_simulation(t_arguments *table);
int			get_time(void);
t_philosophers **initialize_philosophers(t_arguments *table);
pthread_mutex_t *initialize_forks(t_arguments *table);
t_arguments	*initialize(int ac, char **av);
int			str_is_number(char *str);
int			check_arguments_validity(char **av, int ac);

#endif
