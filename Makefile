CC=gcc
CFLAGS=-Wall -Wextra -Werror
RM=rm -f
OBJS=ft_ssl.o \
	 hash_md5.o \
	 padder.o
NAME=ft_ssl

vpath libft.a libft

$(NAME): $(OBJS) -lft
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME) -Llibft -lft

.PHONY: all
all: $(NAME)

.PHONY: clean
clean:
	$(RM) $(OBJS)
	make -C libft clean

.PHONY: fclean
fclean: clean
	$(RM) $(NAME)
	make -C libft fclean

.PHONY: re
re: fclean all

hash_md5.o: hash_md5.h padder.h -lft
	$(CC) $(CFLAGS) -c hash_md5.c

padder.o: padder.h -lft
	$(CC) $(CFLAGS) -c padder.c

libft/libft.a:
	make -C libft
