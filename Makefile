# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hbelle <hbelle@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/10/05 17:52:23 by hbelle            #+#    #+#              #
#    Updated: 2024/02/29 13:36:21 by hbelle           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	=	ircserv

SRCS	=	srcs/main.cpp \
			srcs/parsing/handle_input.cpp \
			srcs/Client.cpp \
			srcs/Server.cpp \
			srcs/Channel.cpp \
			srcs/utils/utils.cpp

SRCS_BOT = srcs/bot/main_bot.cpp \
			srcs/bot/Bot.cpp \
			srcs/bot/TicTacToe.cpp

OBJ_DIR = .o
OBJS = $(addprefix $(OBJ_DIR)/, $(SRCS:%.cpp=%.o))
OBJS_BOT = $(addprefix $(OBJ_DIR)/, $(SRCS_BOT:%.cpp=%.o))

RM	=	rm -f
HEADER =	-I includes

STD	=	-std=c++98

CXXFLAGS= -Wall -Wextra -Werror $(STD) -g

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	@c++ $(CXXFLAGS) $(HEADER) -c $< -o $@
	@if test -s $*.cpp; then \
        echo "\033[01m\033[35mCompiling ⌛\033[00m\
        \033[36m$*.cpp\033[032m  [OK] ✅ \033[00m";\
    else \
        echo "\033[01m\033[33mCompiling \033[00m\
        \033[36m$*.cpp\033[031m  [Error] ❌ \033[00m"; fi

$(NAME): $(OBJS)
	@c++ -o $(NAME) $(OBJS) $(CXXFLAGS) $(HEADER)
	@echo "\033[01m\033[4;33mCompilation done\033[00m\033[1;31m =▶\033[00m\033[1;32m ./${NAME}\033[00m"

bot : $(OBJS_BOT)
	@c++ -o bot $(OBJS_BOT) $(CXXFLAGS) $(HEADER)
	@echo "\033[01m\033[4;33mCompilation done\033[00m\033[1;31m =▶\033[00m\033[1;32m ./bot\033[00m"

all:	${NAME}

clean:
	@${RM} -r $(OBJ_DIR) 
	@echo "\033[01m\033[31mRemoving objects ...\033[00m"

clean_bot:
	@${RM} -r $(OBJ_DIR) 
	@echo "\033[01m\033[31mRemoving objects ...\033[00m"

fclean:	clean
	@${RM} ${NAME}
	@echo "\033[01m\033[31mRemoving exec : ${NAME} ...\033[00m"

fclean_bot:	clean_bot
	@${RM} bot
	@echo "\033[01m\033[31mRemoving exec : bot ...\033[00m"

re:	fclean all

re_bot:	fclean_bot bot

debug: fclean
debug: STD = -std=c++11
debug: CXXFLAGS += -D DEBUG=1
debug: all

.PHONY: all clean fclean re debug