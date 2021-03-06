# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kacoulib <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2017/07/11 14:44:42 by kacoulib          #+#    #+#              #
#    Updated: 2017/07/11 14:44:45 by kacoulib         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	ft_select
CC			=	gcc
RM			=	rm -f
CFLAGS		=	-Wall -Werror -Wextra
LIB			=	-L./libft -lft -lncurses
CPPFLAGS	=	-Iincludes
SRCS		=	src/cursor_handler.c \
				src/keycode_handler.c \
				src/screen_update.c \
				src/screen_display.c \
				src/selection_handler.c \
				src/signal_handler.c \
				src/terminal_handler.c \
				src/ft_select.c

OBJS 		=	$(SRCS:.c=.o)

all:		$(NAME)

$(NAME):	$(OBJS)
			Make -C ./libft
			$(CC) -o $(NAME) $(OBJS) $(LIB) $(CPPFLAGS)

clean:
			$(RM) $(OBJS)
			Make -C ./libft fclean

fclean:		clean
			$(RM) $(NAME)

re:			fclean all

push:
			@git add .
			@echo "Enter Your Commit :"
			@read var1 ; git commit -m "$$var1"
			@echo "Enter The origin :"
			@read var2 ; git push origin "$$var2"

.PHONY:		all clean fclean re push