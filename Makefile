# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: octoross <octoross@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/13 16:58:31 by gtraiman          #+#    #+#              #
#    Updated: 2025/06/07 21:12:49 by octoross         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME        = ircserv

CXX         = c++
CXXFLAGS    = -std=c++98 -Wall -Wextra -Werror -MMD -g3
INCLUDE = include/

SRCS_DIR	= src
SRCS        = $(shell find $(SRCS_DIR) -type f -name '*.cpp')
OBJS_DIR	= objs
OBJS        = $(patsubst $(SRCS_DIR)/%.cpp,$(OBJS_DIR)/%.o,$(SRCS))
DEPS        = $(OBJS:.o=.d)

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)
	
$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.cpp | $(OBJS_DIR)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -I$(INCLUDE) -c $< -o $@


$(OBJS_DIR):
	@mkdir -p $(OBJS_DIR)


-include $(DEPS)

clean:
	@rm -f $(OBJS)
	@rm -f $(DEPS)

fclean:
	@rm -f $(NAME)
	@rm -rf $(OBJS_DIR)

re: fclean all

.PHONY: all clean fclean re
