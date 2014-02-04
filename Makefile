# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                    +:+ +:+         +:+       #
#    By: dlancar <dlancar@student.42.fr>            +#+  +:+       +#+         #
#                                                +#+#+#+#+#+   +#+             #
#    Created: 2013/11/29 09:40:24 by dlancar           #+#    #+#              #
#    Updated: 2014/02/04 15:18:45 by dlancar          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= brainfuck
SRC		= main.c
OBJ		= $(SRC:.c=.o)
LIB		= libft
INCL	= $(LIB)
CMP		= cc
LBFLAG	= -lft
FLAGS	= -Wall -Werror -Wextra -O0 -g

all: $(NAME)

$(NAME): $(OBJ)
	@(cd $(LIB) && $(MAKE))
	@$(CMP) -o $(NAME) $(FLAGS) -I./libft -L./libft $(LBFLAG) $(OBJ)

%.o: %.c
	@$(CMP) -I $(INCL) -o $@ -c $? $(FLAGS)

.PHONY: clean fclean re

clean:
	@(cd $(LIB) && $(MAKE) $@)
	@rm -f $(OBJ)

fclean: clean
	@(cd $(LIB) && $(MAKE) $@)
	@rm -f $(NAME)

re: fclean all

