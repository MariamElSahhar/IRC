# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pbalbino <pbalbino@student.42abudhabi.a    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/06 13:14:25 by melsahha          #+#    #+#              #
#    Updated: 2024/08/06 23:23:53 by pbalbino         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ircserv
CXX = c++
CXXFLAGS = -std=c++98 -Wall -Wextra -Werror

INC_PATH = ./includes/
CFI = -I$(INC_PATH) -I$(INC_PATH)Commands/

SRC_PATH = ./src/
SRC = $(shell find $(SRC_PATH) -name "*.cpp" ! -path "$(SRC_PATH)commands/*")

OBJ_PATH = ./obj/
OBJ = $(patsubst $(SRC_PATH)%.cpp,$(OBJ_PATH)%.o,$(SRC))
OBJ_DIRS = $(sort $(dir $(OBJ)))

$(OBJ_PATH)%.o: $(SRC_PATH)%.cpp
	mkdir -p $(@D)
	@printf "\n$(CY)Generating object...$(RC)\n"
	$(CXX) $(CXXFLAGS) $(CFI) -c $< -o $@
	@printf "\n$(GR)Object ready!$(RC)\n"

RM = rm -rf

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
	valgrind --leak-check=full --track-origins=yes --track-fds=yes --show-leak-kinds=all ./$(NAME) 9999 pass

.PHONY: all clean fclean re run

GR	= \033[32;1m
RE	= \033[31;1m
YE	= \033[33;1m
CY	= \033[36;1m
RC	= \033[0m
