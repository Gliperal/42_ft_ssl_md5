/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl_hash.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwhitlow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/29 18:09:03 by nwhitlow          #+#    #+#             */
/*   Updated: 2019/06/01 14:42:18 by nwhitlow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SSL_HASH_H
# define FT_SSL_HASH_H

# include "padder.h"

void	hash_file(const char *filename, t_hash_algorithm *algorithm, int flags);
void	hash_string(const char *str, t_hash_algorithm *algorithm, int flags);
void	hash_stdin(t_hash_algorithm *algorithm, int print_while_hashing);
int		main_hash(int argc, const char **argv, t_hash_algorithm *algorithm);

#endif
