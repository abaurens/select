# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: abaurens <abaurens@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/10/02 11:43:10 by abaurens          #+#    #+#              #
#    Updated: 2020/12/29 20:56:20 by abaurens         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC      :=	gcc
RM      :=  rm -rf
CP      :=  cp -rf
LINKER  :=  gcc -o

CVERSION := c99

NAME   :=  select

SRCD  :=  src
OBJD  :=  obj

SRCS  :=        \
      parsing.c \
      memory.c  \
      events.c  \
      error.c   \
      main.c    \
      UI.c

OBJS    :=  $(addprefix $(OBJD)/,$(SRCS:.c=.o))
SRCS    :=  $(addprefix $(SRCD)/,$(SRCS))
INCDEPS :=  $(INCDEPS) $(OBJS:.o=.d)

DEFINES :=
DEFINES :=  $(addprefix -D,$(DEFINES))

CFLAGS  :=  -std=$(CVERSION) -MMD -MP -I./include -W -Wall -Wextra -pedantic $(DEFINES) -O3 -Werror
LDFLAGS :=  -lncurses

all:	$(NAME)

$(NAME):	$(OBJS)
	$(LINKER) $(NAME) $(OBJS) $(LDFLAGS)

-include $(INCDEPS)

$(OBJD)/%.o:	$(SRCD)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	$(RM) $(OBJD)

fclean:
	$(RM) $(OBJD)
	$(RM) $(NAME)

re:	fclean all

.PHONY: all clean fclean re
