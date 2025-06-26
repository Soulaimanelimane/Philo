# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: slimane <slimane@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/06/22 21:06:35 by slimane           #+#    #+#              #
#    Updated: 2025/06/26 19:58:35 by slimane          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC=cc

FLAGS=  -Wall -Wextra -Werror 

SRC= main.c ft_atoi.c  

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
	