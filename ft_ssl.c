/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwhitlow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/29 19:01:48 by nwhitlow          #+#    #+#             */
/*   Updated: 2019/05/30 16:09:16 by nwhitlow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl_hash.h"
#include "libft/libft.h"

int	handle_flag_arg(int argc, const char **argv, int *arg, int *flags, const t_hash_algorithm algorithm)
{
	int i;

	i = 1;
	while (argv[*arg][i])
	{
		if (argv[*arg][i] == 'p')
			hash_stdin(algorithm);
		else if (argv[*arg][i] == 'q')
			*flags |= QUIET_MODE;
		else if (argv[*arg][i] == 'r')
			*flags |= REVERSE_MODE;
		else if (argv[*arg][i] == 's')
		{
			if (argv[*arg][i + 1])
				hash_string(argv[*arg] + i + 1, algorithm, *flags);
			else if (*arg + 1 < argc)
			{
				hash_string(argv[*arg + 1], algorithm, *flags);
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
	t_hash_algorithm md5 = hash_algorithms[0];

	flags = 0;
	arg = 1;
	while (arg < argc)
	{
		if ((argv[arg][0] != '-') || (argv[arg][1] == 0))
			break ;
		if (handle_flag_arg(argc, argv, &arg, &flags, md5))
			return (1);
		arg++;
	}
	// TODO If the last flag was q or r, hash stdin
	while (arg < argc)
	{
		hash_file(argv[arg], md5, flags);
		arg++;
	}
}
