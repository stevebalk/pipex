# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sbalk <sbalk@student.fr>                   +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/06/19 15:43:43 by sbalk             #+#    #+#              #
#    Updated: 2023/08/18 15:12:03 by sbalk            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= pipex
LIB_DIR		= libft/
LIB_NAME	= libft.a
CC			= gcc
CFLAGS		= -Werror -Wall -Wextra -g
RM			= rm
SRC_DIR		= src/
OBJ_DIR		= obj/
AR			= ar rcs
INCLUDE		= -I include -I libft/include

# Colors

DEF_COLOR = \033[0;39m
GRAY = \033[0;90m
RED = \033[0;91m
GREEN = \033[0;92m
YELLOW = \033[0;93m
BLUE = \033[0;94m
MAGENTA = \033[0;95m
CYAN = \033[0;96m
WHITE = \033[0;97m

SRC_FILES	=	pipex \


SRC				=	$(addprefix $(SRC_DIR), $(addsuffix .c, $(SRC_FILES)))
OBJ				=	$(addprefix $(OBJ_DIR), $(addsuffix .o, $(SRC_FILES)))


all:		$(NAME)

$(NAME):	$(OBJ)
			@make -C $(LIB_DIR)
			@$(CC) $(CFLAGS) $(OBJ) -L $(LIB_DIR) -lft -o $(NAME)
			@echo "$(GREEN)Created $(NAME)!$(DEF_COLOR)"

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
			@mkdir -p $(OBJ_DIR)
			@echo "$(YELLOW)Compiling: $< $(DEF_COLOR)"
			@$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

clean:
			@$(RM) -rf $(OBJ_DIR)
			@make clean -C $(LIB_DIR)
			@echo "$(BLUE)$(NAME) object files cleaned!$(DEF_COLOR)"

fclean:		clean
			@make fclean -C $(LIB_DIR)
			@$(RM) -f $(NAME)
			@$(RM) -f $(CHECKER)
			@echo "$(CYAN)$(NAME) executable files cleaned!$(DEF_COLOR)"

re:			fclean all
			@echo "$(GREEN)Cleaned and rebuilt everything for $(NAME)push_swap!$(DEF_COLOR)"

norm:
			@norminette $(SRC) include/$(NAME).h| grep -v Norme -B1 || true

git:		fclean
			@git status
			@git add *
			@git status

.PHONY:		all clean fclean re norm git