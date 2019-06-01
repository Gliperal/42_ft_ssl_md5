/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwhitlow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/29 19:01:48 by nwhitlow          #+#    #+#             */
/*   Updated: 2019/05/31 18:32:49 by nwhitlow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl.h"
#include "ft_ssl_hash.h"
#include "libft/libft.h"

void	display_usage(void)
{
	ft_putstr("usage: ./ft_ssl algorithm [-pqr] [-s string] [files ...]\n");
}

int	handle_flag_arg(t_args *args, int *flags, const t_hash_algorithm algorithm)
{
	int i;

	i = 1;
	while (args->argv[args->arg_current][i])
	{
		if (args->argv[args->arg_current][i] == 'p')
			hash_stdin(algorithm, 1);
		else if (args->argv[args->arg_current][i] == 'q')
			*flags |= QUIET_MODE | AWAITING_INPUT;
		else if (args->argv[args->arg_current][i] == 'r')
			*flags |= REVERSE_MODE | AWAITING_INPUT;
		else if (args->argv[args->arg_current][i] == 's')
		{
			*flags |= OVERRIDE_AWAITING_INPUT;
			if (args->argv[args->arg_current][i + 1])
				hash_string(args->argv[args->arg_current] + i + 1, algorithm, *flags);
			else if (args->arg_current + 1 < args->arg_count)
			{
				hash_string(args->argv[args->arg_current + 1], algorithm, *flags);
				args->arg_current += 1;
			}
			else
			{
				ft_printf("ft_ssl: %s: option requires an argument -- s\n", algorithm.id);
				display_usage();
				return (1);
			}
			return (0);
		}
		else
		{
			ft_printf("ft_ssl: %s: illegal option -- %c\n", algorithm.id, args->argv[args->arg_current][i]);
			display_usage();
			return (1);
		}
		i++;
	}
	return (0);
}

int	main(int argc, const char **argv)
{
	int flags;
	t_hash_algorithm algorithm;
	t_args args;

	if (argc > 1 && ft_strequ(argv[1], "md5"))
		algorithm = g_hash_algorithms[0];
	else
	{
		display_usage();
		return (1);
	}
	args.arg_count = argc;
	args.arg_current = 2;
	args.argv = argv;
	flags = 0;
	while (args.arg_current < argc)
	{
		if ((argv[args.arg_current][0] != '-') || (argv[args.arg_current][1] == 0))
			break ;
		if (handle_flag_arg(&args, &flags, algorithm))
			return (1);
		args.arg_current++;
	}
	if (args.arg_current == argc && flags & AWAITING_INPUT && !(flags & OVERRIDE_AWAITING_INPUT))
		hash_stdin(algorithm, 0);
	else
		while (args.arg_current < argc)
		{
			hash_file(argv[args.arg_current], algorithm, flags);
			args.arg_current++;
		}
	return (0);
}
