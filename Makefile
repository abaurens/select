# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: abaurens <abaurens@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/12/03 03:51:56 by abaurens          #+#    #+#              #
#    Updated: 2024/12/04 13:57:47 by abaurens         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC     := gcc
RM     := rm -rf
CP     := cp -rf
LINKER := gcc -o

CVERSION := c99

NAME := select

SRCD := src
OBJD := obj

SRCS := parsing.c \
        memory.c  \
        events.c  \
        error.c   \
        main.c    \
        UI.c

OBJS    :=  $(addprefix $(OBJD)/,$(SRCS:.c=.o))
SRCS    :=  $(addprefix $(SRCD)/,$(SRCS))
INCDEPS :=  $(INCDEPS) $(OBJS:.o=.d)

DEFINES := _GNU_SOURCE                                 \
           VERSION="\"$(shell git describe --tag)\n\""
DEFINES :=  $(addprefix -D,$(DEFINES))

OPTI_LVL := 3

CFLAGS  := -std=$(CVERSION) -MMD -MP -I./include -W -Wall -Wextra -pedantic $(DEFINES)
LDFLAGS := -lncurses -ltinfo

AIO := false
ifeq ($(strip $(AIO)), true)
# Optimize for binary size and force static linkage
	OPTI_LVL := s
	LDFLAGS  := -static $(LDFLAGS)
endif

DEBUG := false
ifeq ($(strip $(DEBUG)), true)
# Optimize for debugging and enable debug symbols
	OPTI_LVL := g
	CFLAGS   := $(CFLAGS) -g
else
# Optimize for speed and enable no tollerance mode on warnings
	CFLAGS   := $(CFLAGS) -Werror
endif

CFLAGS := -O$(OPTI_LVL) $(CFLAGS)

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

.PHONY: all clean fclean re debug
