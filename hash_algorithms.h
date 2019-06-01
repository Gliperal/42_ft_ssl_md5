/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash_algorithms.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwhitlow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/29 18:09:03 by nwhitlow          #+#    #+#             */
/*   Updated: 2019/06/01 14:42:00 by nwhitlow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HASH_ALGORITHMS_H
# define HASH_ALGORITHMS_H

# include <unistd.h>

# include "padder.h"

# define QUIET_MODE 1
# define REVERSE_MODE 2
# define AWAITING_INPUT 4
# define OVERRIDE_AWAITING_INPUT 8

typedef struct					s_hash_algorithm
{
	const char					*id;
	unsigned char				*(*hasher)(t_padder *);
	unsigned int				digest_length;
}								t_hash_algorithm;

unsigned char					*hash_md5(t_padder *message);
unsigned char					*hash_sha256(t_padder *message);

static const t_hash_algorithm	g_hash_algorithms[] =
{
	(t_hash_algorithm) {"md5", &hash_md5, 128},
	(t_hash_algorithm) {"sha256", &hash_sha256, 256},
	(t_hash_algorithm) {NULL, NULL, 0}
};

#endif
