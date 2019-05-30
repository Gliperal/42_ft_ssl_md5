/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwhitlow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/29 19:01:48 by nwhitlow          #+#    #+#             */
/*   Updated: 2019/05/30 12:15:18 by nwhitlow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

#include "padder.h"
#include "ft_ssl.h"
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

static void	hash_padder(t_padder *padder, char *algorithm, const char *subject, int flags)
{
	unsigned char *digest;

	if (!padder)
	{
		write(1, "Malloc failure\n", 15);
		return ;
	}
	digest = hash_md5(padder);
	if (!digest)
	{
		write(1, "Hash failure\n", 13);
		return ;
	}
	if (!(flags & QUIET_MODE) && !(flags & REVERSE_MODE))
	{
		ft_putstr(algorithm);
		write(1, " (", 2);
		ft_putstr(subject);
		write(1, ") = ", 4);
	}
	put_hex_string(digest, 16);
	free(digest);
	if (!(flags & QUIET_MODE) && (flags & REVERSE_MODE))
	{
		write(1, " ", 1);
		ft_putstr(subject);
	}
	write(1, "\n", 1);
}

void	hash_file(const char *filename, char *algorithm, int flags)
{
	t_padder	*padder;
	int			fd;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		write(1, "Open failure\n", 13);
		return ;
	}
	padder = padder_new_file(fd);
	hash_padder(padder, algorithm, filename, flags);
	free(padder);
}

void	hash_string(const char *str, char *algorithm, int flags)
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

void	hash_stdin(char *algorithm)
{
	t_padder *padder;

	padder = padder_new_file(0);
	hash_padder(padder, algorithm, "stdin", QUIET_MODE);
	free(padder);
}

int	handle_flag_arg(int argc, const char **argv, int *arg, int *flags)
{
	int i;

	i = 1;
	while (argv[*arg][i])
	{
		if (argv[*arg][i] == 'p')
			hash_stdin("MD5");
		else if (argv[*arg][i] == 'q')
			*flags |= QUIET_MODE;
		else if (argv[*arg][i] == 'r')
			*flags |= REVERSE_MODE;
		else if (argv[*arg][i] == 's')
		{
			if (argv[*arg][i + 1])
				hash_string(argv[*arg] + i + 1, "MD5", *flags);
			else if (*arg + 1 < argc)
			{
				hash_string(argv[*arg + 1], "MD5", *flags);
				(*arg)++;
			}
			else
			{
				write(1, "ft_ssl: md5: option requires an argument -- s\nusage: md5 [-pqrtx] [-s string] [files ...]\n", 90);
				return (1);
			}
			return (0);
		}
		else
		{
			write(1, "ft_ssl: md5: illegal option -- ", 31);
			write(1, argv[*arg] + i, 1);
			write(1, "\nusage: md5 [-pqrtx] [-s string] [files ...]\n", 45);
			return (1);
		}
		i++;
	}
	return (0);
}

int	main(int argc, const char **argv)
{
	int arg;
	int flags;

	flags = 0;
	arg = 1;
	while (arg < argc)
	{
		if ((argv[arg][0] != '-') || (argv[arg][1] == 0))
			break ;
		if (handle_flag_arg(argc, argv, &arg, &flags))
			return (1);
		arg++;
	}
	while (arg < argc)
	{
		hash_file(argv[arg], "MD5", flags);
		arg++;
	}
}
