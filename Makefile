# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: slimane <slimane@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/06/22 21:06:35 by slimane           #+#    #+#              #
#    Updated: 2025/06/29 15:57:40 by slimane          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC=cc

FLAGS=  -Wall -Wextra -Werror # -g3 -fsanitize=thread

SRC= main.c ft_atoi.c  help.c action.c second_help.c

OBJ=${SRC:.c=.o}

NAME=philo

all:${NAME}

${NAME}:${OBJ}
	${CC} ${CFLAGS} ${OBJ} -o ${NAME} 

%.o: %.c philo.h
	${CC} ${FLAGS} -c $< -o $@ 

clean:
	rm -rf ${OBJ}

fclean:clean
	rm -rf ${NAME}

re: clean all

.PHONY: clean
	