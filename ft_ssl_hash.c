/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl_hash.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwhitlow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/29 19:01:48 by nwhitlow          #+#    #+#             */
/*   Updated: 2019/06/01 15:23:33 by nwhitlow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "hash_algorithms.h"
#include "libft/libft.h"

static void	put_hex_string(unsigned char *str, int len)
{
	unsigned char	hex;
	char			c;
	int				i;

	i = 0;
	while (i < len)
	{
		hex = str[i];
		if (hex > 159)
			c = 'a' - 10 + hex / 16;
		else
			c = '0' + hex / 16;
		write(1, &c, 1);
		if (hex % 16 > 9)
			c = 'a' - 10 + hex % 16;
		else
			c = '0' + hex % 16;
		write(1, &c, 1);
		i++;
	}
}

static void	hash_padder(t_padder *padder, t_hash_algorithm *algorithm, const char *subject, int flags)
{
	unsigned char	*digest;

	if (!padder)
	{
		ft_printf("ft_ssl: %s: %s: Malloc failure\n", algorithm->id, subject);
		return ;
	}
	digest = (*(algorithm->hasher))(padder);
	if (!digest)
	{
		ft_printf("ft_ssl: %s: %s: Hash failure\n", algorithm->id, subject);
		return ;
	}
	if (!(flags & QUIET_MODE) && !(flags & REVERSE_MODE))
	{
		ft_putstr_upper(algorithm->id);
		write(1, " (", 2);
		ft_putstr(subject);
		write(1, ") = ", 4);
	}
	put_hex_string(digest, algorithm->digest_length / 8);
	free(digest);
	if (!(flags & QUIET_MODE) && (flags & REVERSE_MODE))
		ft_printf(" %s", subject);
	write(1, "\n", 1);
}

void		hash_file(const char *file, t_hash_algorithm *algorithm, int flags)
{
	t_padder	*padder;
	int			fd;

	fd = open(file, O_RDONLY);
	if (fd < 0)
	{
		ft_printf("ft_ssl: %s: %s: %s\n", algorithm->id, file, strerror(errno));
		return ;
	}
	padder = padder_new_file(fd, 0);
	hash_padder(padder, algorithm, file, flags);
	free(padder);
}

void		hash_string(const char *str, t_hash_algorithm *algorithm, int flags)
{
	t_padder	*padder;
	char		*subject;

	padder = padder_new_string(str);
	subject = ft_strnew(ft_strlen(str) + 2);
	if (!subject)
	{
		free(padder);
		return ;
	}
	ft_strcpy(subject + 1, str);
	subject[0] = '\"';
	subject[ft_strlen(str) + 1] = '\"';
	hash_padder(padder, algorithm, subject, flags);
	free(padder);
	free(subject);
}

void		hash_stdin(t_hash_algorithm *algorithm, int print_while_hashing)
{
	t_padder *padder;

	padder = padder_new_file(0, print_while_hashing);
	hash_padder(padder, algorithm, "stdin", QUIET_MODE);
	free(padder);
}
