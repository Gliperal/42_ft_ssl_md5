/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwhitlow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/29 19:01:48 by nwhitlow          #+#    #+#             */
/*   Updated: 2019/05/29 20:00:50 by nwhitlow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

#include "padder.h"
#include "ft_ssl.h"
#include "libft/libft.h"

// TODO REMOVE
#include <stdio.h>

void	print_hex_string(unsigned char *str, int len)
{
	unsigned char	hex;
	char			c;

	for (int i = 0; i < len; i++)
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
	}
	write(1, "\n", 1);
}

void	hash_file(const char *filename, int flags)
{
	flags = 0;
	int fd;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		write(1, "Open failure\n", 13);
		return ;
	}
	t_padder *padder = padder_new_file(fd);
	if (!padder)
	{
		write(1, "Malloc failure\n", 15);
		return ;
	}
	unsigned char *digest = hash_md5(padder);
	if (!digest)
	{
		write(1, "Hash failure\n", 13);
		return ;
	}
	write(1, "MD5 (", 5);
	ft_putstr(filename);
	write(1, ") = ", 4);
	print_hex_string(digest, 16);
	free(digest);
	free(padder);
}

void	hash_string(const char *str, int flags)
{
	flags = 0;
	t_padder *padder = padder_new_string(str);
	if (!padder)
	{
		write(1, "Malloc failure\n", 15);
		return ;
	}
	unsigned char *digest = hash_md5(padder);
	if (!digest)
	{
		write(1, "Hash failure\n", 13);
		return ;
	}
	write(1, "MD5 (\"", 6);
	ft_putstr(str);
	write(1, "\") = ", 5);
	print_hex_string(digest, 16);
	free(digest);
	free(padder);
}

void	hash_stdin()
{
//	print and hash stdin (flags = always quiet)
	t_padder *padder = padder_new_file(0);
	if (!padder)
	{
		write(1, "Malloc failure\n", 15);
		return ;
	}
	unsigned char *digest = hash_md5(padder);
	if (!digest)
	{
		write(1, "Hash failure\n", 13);
		return ;
	}
	print_hex_string(digest, 16);
	free(digest);
	free(padder);
}

int	handle_flag_arg(int argc, const char **argv, int *arg, int *flags)
{
	int i;

	i = 1;
	while (argv[*arg][i])
	{
		if (argv[*arg][i] == 'p')
			hash_stdin();
		else if (argv[*arg][i] == 'q')
			*flags |= QUIET_MODE;
		else if (argv[*arg][i] == 'r')
			*flags |= REVERSE_MODE;
		else if (argv[*arg][i] == 's')
		{
			if (argv[*arg][i + 1])
				hash_string(argv[*arg] + i + 1, *flags);
			else if (*arg + 1 < argc)
			{
				hash_string(argv[*arg + 1], *flags);
				(*arg)++;
			}
			else
			{
				write(1, "md5: option requires an argument -- s\nusage: md5 [-pqrtx] [-s string] [files ...]\n", 82);
				return (1);
			}
			return (0);
		}
		else
		{
			write(1, "md5: illegal option -- ", 23);
			write(1, argv[*arg] + i, 1);
			write(1, "\nusage: md5 [-pqrtx] [-s string] [files ...]\n", 45);
			return (1);
		}
		i++;
	}
	return (0);
}

int main(int argc, const char **argv)
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
		hash_file(argv[arg], flags);
		arg++;
	}
}
