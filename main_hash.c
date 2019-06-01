/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_hash.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwhitlow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/29 19:01:48 by nwhitlow          #+#    #+#             */
/*   Updated: 2019/06/01 14:46:20 by nwhitlow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "args.h"
#include "hash_algorithms.h"
#include "ft_ssl_hash.h"
#include "libft/libft.h"

static void	display_usage_hash(void)
{
	ft_putstr("usage: ./ft_ssl algorithm [-pqr] [-s string] [files ...]\n");
}

static int	handle_string_arg(t_args *args, int *flags, t_hash_algorithm *alg, int i)
{
	*flags |= OVERRIDE_AWAITING_INPUT;
	if (args->argv[args->arg_current][i + 1])
		hash_string(args->argv[args->arg_current] + i + 1, alg, *flags);
	else if (args->arg_current + 1 < args->arg_count)
	{
		hash_string(args->argv[args->arg_current + 1], alg, *flags);
		args->arg_current += 1;
	}
	else
	{
		ft_printf("ft_ssl: %s: option requires an argument -- s\n", alg->id);
		display_usage_hash();
		return (1);
	}
	return (0);
}

static int	handle_flag_arg(t_args *args, int *flags, t_hash_algorithm *alg)
{
	int i;

	i = 1;
	while (args->argv[args->arg_current][i])
	{
		if (args->argv[args->arg_current][i] == 'p')
			hash_stdin(alg, 1);
		else if (args->argv[args->arg_current][i] == 'q')
			*flags |= QUIET_MODE | AWAITING_INPUT;
		else if (args->argv[args->arg_current][i] == 'r')
			*flags |= REVERSE_MODE | AWAITING_INPUT;
		else if (args->argv[args->arg_current][i] == 's')
			return (handle_string_arg(args, flags, alg, i));
		else
		{
			ft_printf("ft_ssl: %s: illegal option -- %c\n", alg->id, args->argv[args->arg_current][i]);
			display_usage_hash();
			return (1);
		}
		i++;
	}
	return (0);
}

void	handle_file_args(t_args *args, int flags, t_hash_algorithm *algorithm)
{
	if (args->arg_current == args->arg_count)
	{
		if (flags & AWAITING_INPUT && !(flags & OVERRIDE_AWAITING_INPUT))
			hash_stdin(algorithm, 0);
	}
	else
	{
		while (args->arg_current < args->arg_count)
		{
			hash_file(args->argv[args->arg_current], algorithm, flags);
			args->arg_current++;
		}
	}
}

int	main_hash(int argc, const char **argv, t_hash_algorithm *algorithm)
{
	int		flags;
	t_args	args;

	if (argc == 0)
	{
		hash_stdin(algorithm, 0);
		return (0);
	}
	args.arg_count = argc;
	args.arg_current = 0;
	args.argv = argv;
	flags = 0;
	while (args.arg_current < argc)
	{
		if (argv[args.arg_current][0] != '-')
			break ;
		if (argv[args.arg_current][1] == 0)
			break ;
		if (handle_flag_arg(&args, &flags, algorithm))
			return (1);
		args.arg_current++;
	}
	handle_file_args(&args, flags, algorithm);
	return (0);
}
