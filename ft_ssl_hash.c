/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl_hash.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwhitlow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/29 19:01:48 by nwhitlow          #+#    #+#             */
/*   Updated: 2019/06/01 21:17:01 by nwhitlow         ###   ########.fr       */
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

/*
** NOTE: This function used to be called hash_padder, and subject used to be
** const *char (which it REALLY should be), but I had to fit norme
*/

static void	hp(t_padder *padder, t_hash_algorithm *alg, char *subj, int flags)
{
	unsigned char	*digest;

	if (!padder)
	{
		ft_printf("ft_ssl: %s: %s: Malloc failure\n", alg->id, subj);
		return ;
	}
	digest = (*(alg->hasher))(padder);
	if (!digest)
	{
		ft_printf("ft_ssl: %s: %s: Hash failure\n", alg->id, subj);
		return ;
	}
	if (!(flags & QUIET_MODE) && !(flags & REVERSE_MODE))
	{
		ft_putstr_upper(alg->id);
		write(1, " (", 2);
		ft_putstr(subj);
		write(1, ") = ", 4);
	}
	put_hex_string(digest, alg->digest_length / 8);
	free(digest);
	if (!(flags & QUIET_MODE) && (flags & REVERSE_MODE))
		ft_printf(" %s", subj);
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
	hp(padder, algorithm, (char *)file, flags);
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
	hp(padder, algorithm, subject, flags);
	free(padder);
	free(subject);
}

void		hash_stdin(t_hash_algorithm *algorithm, int print_while_hashing)
{
	t_padder *padder;

	padder = padder_new_file(0, print_while_hashing);
	hp(padder, algorithm, (char *)"stdin", QUIET_MODE);
	free(padder);
}
