# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: abaurens <abaurens@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/12/03 03:51:56 by abaurens          #+#    #+#              #
#    Updated: 2024/12/03 20:11:08 by abaurens         ###   ########.fr        #
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

DEB_CFLAGS := -O0 -g
REL_CFLAGS := -O3 -Werror

CFLAGS  :=  -std=$(CVERSION) -MMD -MP -I./include -W -Wall -Wextra -pedantic $(DEFINES)
LDFLAGS :=  -lncurses

all:	$(NAME)

$(NAME):	$(OBJS)
	$(LINKER) $(NAME) $(OBJS) $(LDFLAGS)

debug:	REL_CFLAGS := $(DEB_CFLAGS)
debug:	$(NAME)

-include $(INCDEPS)

$(OBJD)/%.o:	$(SRCD)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(REL_CFLAGS) -o $@ -c $<

clean:
	$(RM) $(OBJD)

fclean:
	$(RM) $(OBJD)
	$(RM) $(NAME)

re:	fclean all

.PHONY: all clean fclean re debug
