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

NAME = irc
CXX = c++
CXXFLAGS = -std=c++98 -Wall -Werror -Wextra

SRC = $(shell find $(SRC_PATH) -name "*.cpp")

OBJ_PATH =	./obj/
OBJ =	$(addprefix $(OBJ_PATH), $(notdir $(SRC:.cpp=.o)))

$(OBJ_PATH)%.o: %.cpp
			@printf "\n$(CY)Generating object...$(RC)\n"
			mkdir -p $(OBJ_PATH)
			$(CC) $(CFLAGS) -c $< -o $@
			@printf "\n$(GR)Object ready!$(RC)\n"

$(NAME): 	$(OBJ)
			@printf "\n$(CY)Generating executable...$(RC)\n"
			$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME)
			@printf "\n$(GR)Done!$(RC)\n"

all: $(NAME)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

run: $(NAME)
	./$(NAME) $(INPUT_FILE)

leak:	all
	valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all ./$(NAME)

.PHONY: all clean fclean re run
