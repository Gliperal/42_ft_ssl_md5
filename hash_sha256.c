/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash_sha256.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwhitlow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/31 19:31:48 by nwhitlow          #+#    #+#             */
/*   Updated: 2019/06/01 14:27:24 by nwhitlow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "hash_sha256.h"
#include "padder.h"

static void		hash_next1(t_hash_values *v, unsigned char *next)
{
	int				i;

	ft_convert_endian(next, 64, sizeof(int));
	ft_memcpy(v->w, next, 64);
	i = 16;
	while (i < 64)
	{
		v->s0 = ft_right_rotate(v->w[i - 15], 7);
		v->s0 ^= ft_right_rotate(v->w[i - 15], 18) ^ (v->w[i - 15] >> 3);
		v->s1 = ft_right_rotate(v->w[i - 2], 17);
		v->s1 ^= ft_right_rotate(v->w[i - 2], 19) ^ (v->w[i - 2] >> 10);
		v->w[i] = v->w[i - 16] + v->s0 + v->w[i - 7] + v->s1;
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
}

static void		hash_next2(t_hash_values *v)
{
	int				i;

	i = 0;
	while (i < 64)
	{
		v->s1 = ft_right_rotate(v->e, 6) ^ ft_right_rotate(v->e, 11);
		v->s1 ^= ft_right_rotate(v->e, 25);
		v->ch = (v->e & v->f) ^ ((~(v->e)) & v->g);
		v->temp1 = v->h + v->s1 + v->ch + g_k_table[i] + v->w[i];
		v->s0 = ft_right_rotate(v->a, 2) ^ ft_right_rotate(v->a, 13);
		v->s0 ^= ft_right_rotate(v->a, 22);
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
}

static void		hash_next3(t_hash_values *v)
{
	v->h0 += v->a;
	v->h1 += v->b;
	v->h2 += v->c;
	v->h3 += v->d;
	v->h4 += v->e;
	v->h5 += v->f;
	v->h6 += v->g;
	v->h7 += v->h;
}

static int		init(t_hash_values **v, unsigned char **digest)
{
	*v = (t_hash_values *)malloc(sizeof(t_hash_values));
	if (!(*v))
		return (-1);
	*digest = (unsigned char *)malloc(32 * sizeof(char));
	if (!(*digest))
		return (-1);
	(*v)->h0 = 0x6a09e667;
	(*v)->h1 = 0xbb67ae85;
	(*v)->h2 = 0x3c6ef372;
	(*v)->h3 = 0xa54ff53a;
	(*v)->h4 = 0x510e527f;
	(*v)->h5 = 0x9b05688c;
	(*v)->h6 = 0x1f83d9ab;
	(*v)->h7 = 0x5be0cd19;
	return (0);
}

unsigned char	*hash_sha256(t_padder *message)
{
	t_hash_values	*v;
	unsigned char	*next;
	unsigned char	*digest;

	if (init(&v, &digest))
		return (NULL);
	while ((next = padder_next(message, BIG_ENDIAN)))
	{
		hash_next1(v, next);
		hash_next2(v);
		hash_next3(v);
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
