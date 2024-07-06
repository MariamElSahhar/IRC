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

SRC = main.cpp
OBJ = $(SRC:.cpp=.o)

$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME)

all: $(NAME)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

run: $(NAME)
	./$(NAME) $(INPUT_FILE)

.PHONY: all clean fclean re run
