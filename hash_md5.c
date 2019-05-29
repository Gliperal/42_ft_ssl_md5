/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash_md5.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwhitlow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/28 21:51:11 by nwhitlow          #+#    #+#             */
/*   Updated: 2019/05/28 22:44:40 by nwhitlow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft/libft.h"
#include "hash_md5.h"

static int	left_rotate(int n, int dist)
{
	return ((n << dist) | (n >> (32 - dist)));
}

static void	hash_next(t_hash_values *h, unsigned char *next)
{
	int i;
	unsigned int chunk[16];

	ft_memcpy(chunk, next, 64);
	h->a = h->hash_a;
	h->b = h->hash_b;
	h->c = h->hash_c;
	h->d = h->hash_d;
	i = 0;
	while (i < 63)
	{
		if (i < 16)
			h->f = (h->b & h->c) | ((~h->b) & h->d);
		else if (i < 32)
			h->f = (h->d & h->b) | ((~h->d) & h->c);
		else if (i < 48)
			h->f = h->b ^ h->c ^ h->d;
		else
			h->f = h->c ^ (h->b | (~h->d));
		h->g = (g_g_table[i / 16][0] * i + g_g_table[i / 16][1]) % 16;
		h->f = h->f + h->a + g_k_table[i] + chunk[g];
		h->a = h->d;
		h->d = h->c;
		h->c = h->b;
		h->b = h->b + leftrotate(h->f, g_s_table[i]);
		i++;
	}
	h->hash_a = h->hash_a + h->a;
	h->hash_b = h->hash_b + h->b;
	h->hash_c = h->hash_c + h->c;
	h->hash_d = h->hash_d + h->d;
}

unsigned char	*hash_md5(unsigned char *message)
{
	t_hash_values *h;
	unsigned char *digest;

	h = (t_has_values *)malloc(sizeof(t_hash_values));
	if (!h)
		return (NULL);
	digest = (char *)malloc(16 * sizeof(char));
	h->hash_a = 0x67452301;
	h->hash_b = 0xefcdab89;
	h->hash_c = 0x98badcfe;
	h->hash_d = 0x10325476;
	while (reader.hasNext())
		hash_next(h, reader.next(64 bytes));
	ft_memcpy(digest, h->a, 4);
	ft_memcpy(digest + 4, h->b, 4);
	ft_memcpy(digest + 8, h->c, 4);
	ft_memcpy(digest + 12, h->d, 4);
	return (digest);
}
