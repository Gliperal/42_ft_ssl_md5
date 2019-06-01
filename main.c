/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwhitlow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/29 19:01:48 by nwhitlow          #+#    #+#             */
/*   Updated: 2019/06/01 14:49:18 by nwhitlow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "args.h"
#include "hash_algorithms.h"
#include "ft_ssl_hash.h"
#include "libft/libft.h"

static const t_hash_algorithm	*get_hash_algorithm(const char *name)
{
	int i;

	i = 0;
	while (g_hash_algorithms[i].id)
	{
		if (ft_strequ(g_hash_algorithms[i].id, name))
			return (g_hash_algorithms + i);
		i++;
	}
	return (NULL);
}

static void						display_commands(void)
{
	int i;

	ft_putstr("\nStandard commands:\n");
	ft_putstr("\nMessage Digest commands:\n");
	i = 0;
	while (g_hash_algorithms[i].id)
	{
		ft_putendl(g_hash_algorithms[i].id);
		i++;
	}
	ft_putstr("\nCipher commands:\n");
}

int								main(int argc, const char **argv)
{
	t_hash_algorithm *algorithm;

	if (argc == 1)
	{
		ft_putstr("usage: ft_ssl command [command opts] [command args]\n");
		return (1);
	}
	else
	{
		algorithm = (t_hash_algorithm *)get_hash_algorithm(argv[1]);
		if (algorithm)
			return (main_hash(argc - 2, argv + 2, algorithm));
		else
		{
			ft_printf("ft_ssl: Error: '%s' is an invalid command.\n", argv[1]);
			display_commands();
			return (1);
		}
	}
}
