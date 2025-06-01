# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gtraiman <gtraiman@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/13 16:58:31 by gtraiman          #+#    #+#              #
#    Updated: 2025/05/31 19:47:28 by gtraiman         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME        = ircserv

CXX         = c++
CXXFLAGS    = -std=c++98 -Wall -Wextra -Werror -g3 -I$
INCLUDE = include

SRCS        = $(shell find $(SRC_DIR) -type f -name '*.cpp')

OBJS        = $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

src/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -I$(INCLUDE) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
