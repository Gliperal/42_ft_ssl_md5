/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl_hash.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwhitlow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/29 18:09:03 by nwhitlow          #+#    #+#             */
/*   Updated: 2019/05/31 20:17:01 by nwhitlow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SSL_HASH_H
# define FT_SSL_HASH_H

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

void	hash_file(const char *filename, t_hash_algorithm *algorithm, int flags);
void	hash_string(const char *str, t_hash_algorithm *algorithm, int flags);
void	hash_stdin(t_hash_algorithm *algorithm, int print_while_hashing);

#endif
