/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   padder.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwhitlow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/29 12:01:29 by nwhitlow          #+#    #+#             */
/*   Updated: 2019/05/29 16:48:04 by nwhitlow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>

#include "libft/libft.h"
#include "padder.h"

t_padder	*padder_new_file(int fd)
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
	return (padder);
}

t_padder	*padder_new_string(char *message)
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

static int				read_into_buffer(t_padder *padder, unsigned char *buff)
{
	int	bytes_read;
	int total_bytes_read;

	total_bytes_read = 0;
	while ((bytes_read = read(padder->fd, buff, 64 - total_bytes_read)) > 0)
		total_bytes_read += bytes_read;
	if (bytes_read == -1)
		return (-1);
	padder->size_so_far += total_bytes_read * 8;
	return (total_bytes_read);
}

static unsigned char	*next_fd(t_padder *padder)
{
	unsigned char	buffer[64];
	int				bytes_read;

	if (padder->last == 2)
		return (NULL);
	bytes_read = read_into_buffer(padder, buffer);
	if (bytes_read == -1)
		return (NULL);
	if (bytes_read == 64)
		return (ft_memdup(buffer, 64));
	ft_bzero(buffer + bytes_read, 64 - bytes_read);
	if (!padder->last)
		buffer[bytes_read] = 0x80;
	if (bytes_read < 56)
	{
		ft_memcpy(buffer + 56, &(padder->size_so_far), 8);
		padder->last = 2;
	}
	else
		padder->last = 1;
	return (ft_memdup(buffer, 64));
}

static unsigned char	*next_str(t_padder *padder)
{
	if (padder)
		return (NULL);
	return (NULL);
}

unsigned char	*padder_next(t_padder *padder)
{
	if (padder->fd == -1)
		return (next_str(padder));
	else
		return (next_fd(padder));
}
