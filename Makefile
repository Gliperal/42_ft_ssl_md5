CC=gcc
CFLAGS=-Wall -Wextra -Werror
RM=rm -f
OBJS=main.o \
	 ft_ssl_hash.o \
	 hash_md5.o \
	 hash_sha256.o \
	 main_hash.o \
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

ft_ssl_hash.o: padder.h hash_algorithms.h -lft

hash_md5.o: hash_md5.h padder.h -lft

hash_sha256.o: hash_sha256.h padder.h -lft

main.o: args.h hash_algorithms.h ft_ssl_hash.h -lft

main_hash.o: args.h hash_algorithms.h ft_ssl_hash.h -lft

padder.o: padder.h -lft

libft/libft.a:
	make -C libft
