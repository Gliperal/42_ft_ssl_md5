/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   padder.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nwhitlow <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/29 11:59:46 by nwhitlow          #+#    #+#             */
/*   Updated: 2019/05/31 17:58:47 by nwhitlow         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PADDER_H
# define PADDER_H

typedef struct			s_padder
{
	int					fd;
	const char			*str;
	unsigned long		size_so_far;
	int					last;
	int					print_while_padding;
}						t_padder;

t_padder				*padder_new_file(int fd, int print_while_padding);
t_padder				*padder_new_string(const char *message);
unsigned char			*padder_next(t_padder *padder);

#endif
