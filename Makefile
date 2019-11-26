# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: agelloz <agelloz@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/07/19 11:32:25 by agelloz           #+#    #+#              #
#    Updated: 2019/11/26 19:25:43 by agelloz          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC = gcc
NAME = lem-in
CFLAGS = -Wall -Wextra -Werror -O3 -ffreestanding -fno-builtin -flto
#CFLAGS = -Wall -Wextra -g3 -fsanitize=address,undefined -Wpadded
LIBH = libft/inc
INC = inc/
HEAD = $(INC)lem_in.h
LIBA = libft/libft.a
SRC_DIR = src/

SRC_FILES += lem_in.c 
SRC_FILES += parsing.c 
SRC_FILES += parsing2.c 
SRC_FILES += parsing_tests.c 
SRC_FILES += parsing_tests2.c 
SRC_FILES += graph.c 
SRC_FILES += print.c 
SRC_FILES += exit.c
SRC_FILES += exit_parsing.c
SRC_FILES += edmonds_karp.c
SRC_FILES += bfs.c
SRC_FILES += bfs_tools.c
SRC_FILES += bfs_disjoint_paths.c
SRC_FILES += init_output.c
SRC_FILES += solver.c
SRC_FILES += flow_ants.c
SRC_FILES += bfs_reset_tools.c
SRC_FILES += print_result.c
SRC_FILES += all_ants_moved.c
SRC_FILES += cytoscape_visualizer.c

SRC = $(addprefix $(SRC_DIR), $(SRC_FILES))
OBJ = $(SRC:%.c=%.o)

all: $(NAME)

$(NAME): $(LIBA) $(OBJ) 
	$(CC) $(CFLAGS) -I$(LIBH) -I$(INC) -Llibft -lft -o $@ $(OBJ)
	printf "$(GREEN)created: $@\n$(NC)"

$(LIBA): FORCE
	$(MAKE) -C libft

FORCE:

%.o: %.c $(HEAD)
	$(CC) $(CFLAGS) -I$(LIBH) -I$(INC) -o $@ -c $<
	printf "$(CYAN)compiling: $<\n$(NC)"

clean:
	$(MAKE) -C libft fclean
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)
	printf "$(RED)deleted: $(NAME)\n$(NC)"

re: fclean all

CYAN=\033[0;36m
GREEN=\033[0;32m
RED=\033[0;31m
NC=\033[0m

.PHONY: all clean fclean re
.SILENT:
