#include "../includes/philosophers.h"

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

void	check_meal(t_philosophers *philosophers)
{
	int	i;

	i = 0;
	pthread_mutex_lock(&philosophers->mutex_eating);
	while (i < philosophers->table->number_of_meals)
	{
		if (philosophers->table->philosophers[i]->meal_counter
			>= philosophers->table->number_of_meals)
		{
			if (i == philosophers->table->number_of_philosophers - 1)
			{
				pthread_mutex_lock(&philosophers->table->mutex_write);
				philosophers->table->end = 0;
			}
			i++;
		}
		else
			break ;
	}
	pthread_mutex_unlock(&philosophers->mutex_eating);
}

void	*monitor(void *argument)
{
	t_philosophers	*philosophers;

	philosophers = ((t_philosophers *)argument);
	while (philosophers->table->end)
	{
		if (!philosophers->is_eating
			&& get_time() - philosophers->last_meal_eaten 
			>= philosophers->table->time_to_die)
		{
			pthread_mutex_lock(&philosophers->mutex_eating);
			message(philosophers->table,
				philosophers->philosopher_number, "died 💀 🪦");
			philosophers->table->end = 0;
			pthread_mutex_unlock(&philosophers->mutex_eating);
		}
		if (philosophers->table->number_of_meals
				&& philosophers->meal_counter 
				>= philosophers->table->number_of_meals)
			check_meal(philosophers);
		usleep(100);
	}
	return (NULL);
}

void	message(t_arguments *table, int philo_number, char *message)
{
	pthread_mutex_lock(&table->mutex_write);
	printf("[%d]\tPhilosopher %d %s\n", get_time() - table->time_starter,
		philo_number + 1, message);
	if (message[0] != 'd')
		pthread_mutex_unlock(&table->mutex_write);
}

void	ft_usleep(u_int64_t time_in_milliseconde)
{
	u_int64_t	time_start;

	time_start = 0;
	time_start = get_time();
	while ((get_time() - time_start) < time_in_milliseconde)
		usleep(time_in_milliseconde / 10);
}

void	take_fork(t_philosophers *philosophers)
{
	pthread_mutex_lock
		(&philosophers->table->mutex_forks[philosophers->left_fork]);
	message(philosophers->table, philosophers->philosopher_number,
		"has taken the left fork 🍴");
	pthread_mutex_lock
		(&philosophers->table->mutex_forks[philosophers->right_fork]);
	message(philosophers->table, philosophers->philosopher_number,
		"has taken the right fork 🍴");
	pthread_mutex_lock(&philosophers->mutex_eating);
	philosophers->last_meal_eaten = get_time();
	philosophers->is_eating = 1;
	message(philosophers->table, philosophers->philosopher_number,
		"is eating 😋 🍝");
	ft_usleep(philosophers->table->time_to_eat);
	philosophers->is_eating = 0;
	philosophers->meal_counter++;
	pthread_mutex_unlock(&philosophers->mutex_eating);
}

void	*routine(void *argument)
{
	t_philosophers	*philosophers;

	philosophers = ((t_philosophers *)argument);
	while (philosophers->table->end)
	{
		take_fork(philosophers);
		pthread_mutex_unlock
			(&philosophers->table->mutex_forks[philosophers->left_fork]);
		pthread_mutex_unlock
			(&philosophers->table->mutex_forks[philosophers->left_fork]);
		message(philosophers->table,
			philosophers->philosopher_number, "is_sleeping 😴 💤");
		ft_usleep(philosophers->table->time_to_sleep);
		message(philosophers->table,
			philosophers->philosopher_number, "is thinking 🤔 💭");
	}
	return (NULL);
}

void	start_simulation(t_arguments *table)
{
	int	i;

	i = 0;
	while (i < table->number_of_philosophers)
	{
		table->philosophers[i]->last_meal_eaten = get_time();
		pthread_create(&table->philosophers[i]->philo_thread, NULL, routine,
				(void *)table->philosophers[i]);
		i++;
		usleep(100);
	}
	i = 0;
	while (i < table->number_of_philosophers)
	{
		pthread_create(&table->philosophers[i]->monitor_thread, NULL, monitor,
		(void *)table->philosophers[i]);
		i++;
		usleep(100);
	}
}

int	get_time(void)
{
	static struct timeval tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * (u_int64_t)1000 + (tv.tv_usec / 1000));
}	

t_philosophers **initialize_philosophers(t_arguments *table)
{
	t_philosophers	**philosophers;
	int	i;

	philosophers = (t_philosophers **)malloc(sizeof(t_philosophers) * 1);
	if (!philosophers)
		return (NULL);
	i = 0;
	while (i < (int)table->number_of_philosophers)
	{
		philosophers[i] = (t_philosophers *)malloc(sizeof(t_philosophers) * 1);
		if (!philosophers[i])
			return (NULL);
		if (pthread_mutex_init(&philosophers[i]->mutex_eating, NULL))
			return (NULL);
		philosophers[i]->philosopher_number = i;
		philosophers[i]->table = table;
		philosophers[i]->is_eating = 0;
		philosophers[i]->meal_counter = 0;
		philosophers[i]->left_fork = i;
		philosophers[i]->right_fork = (i + 1) % table->number_of_philosophers;
		i++;
	}
	return (philosophers);
}

pthread_mutex_t *initialize_forks(t_arguments *table)
{
	pthread_mutex_t	*mutex_forks;
	int	i;

	i = 0;
	if (table->number_of_philosophers == -1 || table->time_to_die == -1
		|| table->time_to_eat == -1 || table->time_to_sleep == -1
		|| table->number_of_meals == -1)
	{
		printf(INV_ERROR);
		return (NULL);
	}
	mutex_forks = malloc(sizeof(pthread_mutex_t) 
		* table->number_of_philosophers);
	if (!mutex_forks)
		return (NULL);
	while (i < table->number_of_philosophers)
	{
		pthread_mutex_init(&mutex_forks[i], NULL);
		i++;
	}
	return (mutex_forks);
}

t_arguments	*initialize(int ac, char **av)
{
	t_arguments	*table;

	table = malloc(sizeof(t_arguments) * 1);
	if (!table)
		return (NULL);
	table->number_of_philosophers = (u_int64_t)ft_atoi(av[1]);
	table->time_to_die = (u_int64_t)ft_atoi(av[2]);
	table->time_to_eat = (u_int64_t)ft_atoi(av[3]);
	table->number_of_meals = 0;
	if (ac == 6)
		table->number_of_meals = ft_atoi(av[5]);
	pthread_mutex_init(&table->mutex_write, NULL);
	table->time_starter = get_time();
	table->mutex_forks = initialize_forks(table);
	table->philosophers = initialize_philosophers(table);
	if (!table->philosophers || !table->mutex_forks)
		return (NULL);
	return (table);
}

int	str_is_number(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (1);
		i++;
	}
	return (0);
}

int	check_arguments_validity(char **av, int ac)
{
	int	i;

	i = 1;
	if (ac < 5 || ac > 6)
	{
		printf(ARG_ERROR);
		return (1);
	}
	while (i < ac)
	{
		if (av[i][0] == '-'|| str_is_number(av[i])
			|| ft_atoi(av[i]) == 0)
		{
			printf(INV_ERROR);
			return (1);
		}
		i++;
	}
	return (0);
}

int	main(int ac, char ** av)
{
	t_arguments	*table;

	if (check_arguments_validity(av, ac))
		return (1);
	table = initialize(ac, av);
	if (!table)
		return (1);
	table->time_starter = get_time();
	start_simulation(table);
	while (table->end)
		continue ;
	return (0);
}