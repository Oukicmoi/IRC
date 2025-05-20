# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: octoross <octoross@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/13 16:58:31 by gtraiman          #+#    #+#              #
#    Updated: 2025/05/20 16:43:38 by octoross         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME        = ircserv

CXX         = c++
CXXFLAGS    = -std=c++98 -Wall -Wextra -Werror -g3
INCLUDE = include

SRCS        = src/main.cpp \
			  src/Server/core.cpp \
			  src/Server/run.cpp \
			  src/Server/up.cpp \
			  src/Server/handle_clients.cpp \
			  src/Server/commands/cap.cpp \
			  src/Server/commands/join.cpp \
			  src/Server/commands/msg.cpp \
			  src/Server/commands/part.cpp \
			  src/Server/commands/privmsg.cpp \
			  src/Server/commands/topic.cpp \
              src/Channel.cpp \
              src/IRCMessage.cpp \
              src/User.cpp \

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
