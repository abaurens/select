# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: abaurens <abaurens@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/12/03 03:51:56 by abaurens          #+#    #+#              #
#    Updated: 2024/12/05 22:00:12 by abaurens         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CC     := gcc
RM     := rm -rf
CP     := cp -rf
LINKER := gcc -o

NAME := select

CVERSION := ansi
OPTI_LVL := 3

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

INCLUDES := ./include
INCLUDES :=  $(addprefix -I,$(INCLUDES))


DEFINES := _GNU_SOURCE                                 \
           VERSION="\"$(shell git describe --tag)\n\""
DEFINES :=  $(addprefix -D,$(DEFINES))

CFLAGS  := -MMD -MP -W -Wall -Wextra -pedantic $(INCLUDES) $(DEFINES)
LDFLAGS := -lncurses -ltinfo


ifeq ($(strip $(CVERSION)), ansi)
  CFLAGS := -ansi $(CFLAGS)
else
  CFLAGS := -std=$(CVERSION) $(CFLAGS)
endif

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

re:	fclean $(NAME)

.PHONY: clean fclean re debug
