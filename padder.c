/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   padder.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwhitlow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/29 12:01:29 by nwhitlow          #+#    #+#             */
/*   Updated: 2019/06/01 12:30:18 by nwhitlow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>

#include "libft/libft.h"
#include "padder.h"

t_padder		*padder_new_file(int fd, int print_while_padding)
{
	t_padder *padder;

	if (fd < 0)
		return (NULL);
	padder = (t_padder *)malloc(sizeof(t_padder));
	if (!padder)
		return (NULL);
	padder->fd = fd;
	padder->str = NULL;
	padder->size_so_far = 0;
	padder->last = 0;
	padder->print_while_padding = print_while_padding;
	return (padder);
}

t_padder		*padder_new_string(const char *message)
{
	t_padder *padder;

	if (!message)
		return (NULL);
	padder = (t_padder *)malloc(sizeof(t_padder));
	if (!padder)
		return (NULL);
	padder->fd = -1;
	padder->str = message;
	padder->size_so_far = 0;
	padder->last = 0;
	return (padder);
}

static int		read_file_into_buffer(t_padder *padder, unsigned char *buff)
{
	int	bytes_read;
	int total_bytes_read;

	total_bytes_read = 0;
	while ((bytes_read = read(padder->fd, buff, 64 - total_bytes_read)) > 0)
		total_bytes_read += bytes_read;
	if (bytes_read == -1)
		return (-1);
	padder->size_so_far += total_bytes_read;
	if (padder->print_while_padding)
		write(1, buff, total_bytes_read);
	return (total_bytes_read);
}

static int		read_str_into_buffer(t_padder *padder, unsigned char *buff)
{
	int bytes_remaining;

	bytes_remaining = ft_strlen(padder->str) - padder->size_so_far;
	if (bytes_remaining >= 64)
	{
		ft_memcpy(buff, padder->str + padder->size_so_far, 64);
		padder->size_so_far += 64;
		return (64);
	}
	padder->size_so_far += bytes_remaining;
	ft_memcpy(buff, padder->str, bytes_remaining);
	return (bytes_remaining);
}

unsigned char	*padder_next(t_padder *padder, int endian)
{
	unsigned char	buffer[64];
	int				bytes_read;

	if (padder->last == 2)
		return (NULL);
	if (padder->fd == -1)
		bytes_read = read_str_into_buffer(padder, buffer);
	else
		bytes_read = read_file_into_buffer(padder, buffer);
	if (bytes_read == -1)
		return (NULL);
	if (bytes_read == 64)
		return (ft_memdup(buffer, 64));
	ft_bzero(buffer + bytes_read, 64 - bytes_read);
	if (!padder->last)
		buffer[bytes_read] = 0x80;
	if (bytes_read < 56)
	{
		padder->size_so_far *= 8;
		ft_number_cpy(buffer + 56, &(padder->size_so_far), 8, endian);
		padder->last = 2;
	}
	else
		padder->last = 1;
	return (ft_memdup(buffer, 64));
}
