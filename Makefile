# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: melsahha <melsahha@student.42abudhabi.a    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/06 13:14:25 by melsahha          #+#    #+#              #
#    Updated: 2024/07/06 13:14:36 by melsahha         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ircserv
CXX = c++
CXXFLAGS = -std=c++98 -Wall -Werror -Wextra

INC_PATH = ./includes/
CFI = -I$(INC_PATH)

SRC_PATH = ./src/
SRC = $(shell find $(SRC_PATH) -name "*.cpp" ! -path "$(SRC_PATH)commands/*")

OBJ_PATH =	./obj/
OBJ = $(SRC:$(SRC_PATH)%.cpp=$(OBJ_PATH)%.o)

RM = rm -rf

$(OBJ_PATH)%.o: $(SRC_PATH)%.cpp
			@printf "\n$(CY)Generating object...$(RC)\n"
			mkdir -p $(OBJ_PATH)
			$(CC) $(CFLAGS) $(CFI) -c $< -o $@
			@printf "\n$(GR)Object ready!$(RC)\n"

$(NAME): 	$(OBJ)
			@printf "\n$(CY)Generating executable...$(RC)\n"
			$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME) $(CFI)
			@printf "\n$(GR)Done!$(RC)\n"

all: $(NAME)

clean:
	$(RM) $(OBJ_PATH)

fclean: clean
	$(RM) $(NAME)

re: fclean all

run: $(NAME)
	./$(NAME) $(INPUT_FILE)

leak:	all
	valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all ./$(NAME)

.PHONY: all clean fclean re run

GR	= \033[32;1m
RE	= \033[31;1m
YE	= \033[33;1m
CY	= \033[36;1m
RC	= \033[0m
