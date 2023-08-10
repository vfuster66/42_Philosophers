/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfuster- <vfuster-@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/03 18:22:16 by vfuster-          #+#    #+#             */
/*   Updated: 2023/08/07 15:11:26 by vfuster-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	ft_bzero(void *s, size_t n)
{
	unsigned char	*dest;
	size_t			i;

	dest = s;
	i = 0;
	while (i++ < n)
		*dest++ = 0;
}

int	ft_atoi(char const *nptr)
{
	int		i;
	long	number;
	long	sign;

	i = 0;
	sign = 1;
	number = 0;
	while (nptr[i] == ' ' || ((9 <= nptr[i]) && (nptr[i] <= 13)))
		i++;
	if (nptr[i] == '-')
	{
		sign = -1;
		i++;
	}
	else if (nptr[i] == '+')
		i++;
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		number = number * 10 + nptr[i] - '0';
		i++;
	}
	if (number * sign > INT_MAX)
		return (INT_MAX);
	else if (number *sign < INT_MIN)
		return (INT_MIN);
	else
		return ((int)(number * sign));
}

void	*ft_memset(void *s, int c, size_t n)
{
	unsigned char	*new_dest;
	unsigned char	src;
	size_t			i;

	i = 0;
	new_dest = s;
	src = c;
	while (i++ < n)
		*new_dest++ = src;
	return (s);
}

void	*ft_calloc(size_t count, size_t size)
{
	void	*pnt;

	if (count == 0 || size == 0)
	{
		count = 1;
		size = 1;
	}
	if (count > (count * size) / size)
		return (NULL);
	pnt = malloc(count * size);
	if (pnt == NULL)
		return (NULL);
	ft_memset((unsigned char *)pnt, 0, count * size);
	return (pnt);
}

void	print_status(t_philo *philosopher, char *state)
{
	if (philosopher->args->game_over == 0)
	{
		pthread_mutex_lock(&philosopher->args->print_mutex);
		printf("%lld\t%d\t%s\n",
			get_timestamp(philosopher->args), philosopher->id, state);
		pthread_mutex_unlock(&philosopher->args->print_mutex);
	}
}
