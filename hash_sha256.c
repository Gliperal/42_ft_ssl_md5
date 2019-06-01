/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash_sha256.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwhitlow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/31 19:31:48 by nwhitlow          #+#    #+#             */
/*   Updated: 2019/06/01 12:33:25 by nwhitlow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "hash_sha256.h"
#include "padder.h"

static unsigned int	right_rotate(unsigned int n, int dist)
{
	return ((n >> dist) | (n << (32 - dist)));
}

static void	convert_to_big_endian(unsigned char *data, int size_in_bytes)
{
	unsigned char tmp;
	for (int i = 0; i < size_in_bytes; i += 4)
	{
		tmp = data[i];
		data[i] = data[i + 3];
		data[i + 3] = tmp;
		tmp = data[i + 1];
		data[i + 1] = data[i + 2];
		data[i + 2] = tmp;
	}
}

static void			hash_next(t_hash_values *v, unsigned char *next)
{
	int				i;
	unsigned int	w[64];

	convert_to_big_endian(next, 64);
	ft_memcpy(w, next, 64);
	i = 16;
	while (i < 64)
	{
		v->s0 = right_rotate(w[i - 15], 7) ^ right_rotate(w[i - 15], 18) ^ (w[i - 15] >> 3);
		v->s1 = right_rotate(w[i - 2], 17) ^ right_rotate(w[i - 2], 19) ^ (w[i - 2] >> 10);
		w[i] = w[i - 16] + v->s0 + w[i - 7] + v->s1;
		i++;
	}
	v->a = v->h0;
	v->b = v->h1;
	v->c = v->h2;
	v->d = v->h3;
	v->e = v->h4;
	v->f = v->h5;
	v->g = v->h6;
	v->h = v->h7;
	i = 0;
	while (i < 64)
	{
		v->s1 = right_rotate(v->e, 6) ^ right_rotate(v->e, 11) ^ right_rotate(v->e, 25);
		v->ch = (v->e & v->f) ^ ((~(v->e)) & v->g);
		v->temp1 = v->h + v->s1 + v->ch + g_k_table[i] + w[i];
		v->s0 = right_rotate(v->a, 2) ^ right_rotate(v->a, 13) ^ right_rotate(v->a, 22);
		v->maj = (v->a & v->b) ^ (v->a & v->c) ^ (v->b & v->c);
		v->temp2 = v->s0 + v->maj;
		v->h = v->g;
		v->g = v->f;
		v->f = v->e;
		v->e = v->d + v->temp1;
		v->d = v->c;
		v->c = v->b;
		v->b = v->a;
		v->a = v->temp1 + v->temp2;
		i++;
	}
	v->h0 += v->a;
	v->h1 += v->b;
	v->h2 += v->c;
	v->h3 += v->d;
	v->h4 += v->e;
	v->h5 += v->f;
	v->h6 += v->g;
	v->h7 += v->h;
}

unsigned char		*hash_sha256(t_padder *message)
{
	t_hash_values	*v;
	unsigned char	*next;
	unsigned char	*digest;

	v = (t_hash_values *)malloc(sizeof(t_hash_values));
	if (!v)
		return (NULL);
	digest = (unsigned char *)malloc(32 * sizeof(char));
	if (!digest)
		return (NULL);
	v->h0 = 0x6a09e667;
	v->h1 = 0xbb67ae85;
	v->h2 = 0x3c6ef372;
	v->h3 = 0xa54ff53a;
	v->h4 = 0x510e527f;
	v->h5 = 0x9b05688c;
	v->h6 = 0x1f83d9ab;
	v->h7 = 0x5be0cd19;
	while ((next = padder_next(message, BIG_ENDIAN)))
	{
		hash_next(v, next);
		free(next);
	}
	ft_number_cpy(digest, &(v->h0), 4, BIG_ENDIAN);
	ft_number_cpy(digest + 4, &(v->h1), 4, BIG_ENDIAN);
	ft_number_cpy(digest + 8, &(v->h2), 4, BIG_ENDIAN);
	ft_number_cpy(digest + 12, &(v->h3), 4, BIG_ENDIAN);
	ft_number_cpy(digest + 16, &(v->h4), 4, BIG_ENDIAN);
	ft_number_cpy(digest + 20, &(v->h5), 4, BIG_ENDIAN);
	ft_number_cpy(digest + 24, &(v->h6), 4, BIG_ENDIAN);
	ft_number_cpy(digest + 28, &(v->h7), 4, BIG_ENDIAN);
	free(v);
	return (digest);
}
