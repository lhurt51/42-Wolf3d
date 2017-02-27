# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lhurt <marvin@42.fr>                       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2017/01/23 18:39:06 by lhurt             #+#    #+#              #
#    Updated: 2017/01/23 18:39:08 by lhurt            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC =		gcc

FLAG =		-Wall -Wextra -Werror -o

LX =		-lmlx

MINLX =		./minilibx

INC =		-I $(MINLX)

LIB =		-L $(MINLX)

OPENGL =	-framework OpenGl

APPKIT =	-framework AppKit

LIBFT =		libft

COMP =		make -C $(LIBFT) re

COMPC =		make -C $(LIBFT) clean

COMPF =		make -C $(LIBFT) fclean

NAME =		wolf3d

SRCS =		$(LIBFT)/libft.a \
			main.c	\
			read_xmp.c \
			read_xpm1.c \
			read_colors.c \
			read_files.c \
			read_map.c \
			tex_pallet.c \
			load_files.c \
			run_game.c \
			sprite_casting.c \
			run_casting.c \
			find_tex.c \
			draw_floor.c \
			xpm_to_env.c \
			mouse_hooks.c \
			keys_and_mouse.c \
			key_hooks.c \
			tools.c \
			tools1.c \
			tools2.c \

THREAD =	-lpthread

all: 	$(NAME)

$(NAME):
		@$(COMP)
		@$(CC) $(INC) $(LIB) $(LX) $(OPENGL) $(APPKIT) $(FLAG) $(NAME) $(SRCS) $(THREAD)
		@echo "$(NAME) & $(LIBFT).a: has been created."

clean:
		@$(COMPC)
		@echo "$(LIBFT) has been cleaned."

fclean:	clean
		@$(COMPF)
		@/bin/rm -f $(NAME)
		@echo "$(NAME) & $(LIBFT).a: has been erased."

re: 	fclean all

.PHONY: all clean fclean re
