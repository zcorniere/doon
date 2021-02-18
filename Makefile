##
## A Makefile with faith
## Hu 2.0
##

NAME := doon
HEADP := ./include/

SRC_FOLDER := source
OBJ_FOLDER := .object

CC := clang++

LANG := .cpp
VPATH := $(SRC_FOLDER) $(SRC_FOLDER)/objects
SRC := $(notdir $(shell find $(SRC_FOLDER) -name '*$(LANG)'))
OBJ := $(addprefix $(OBJ_FOLDER)/,$(SRC:$(LANG)=.o))

DEP_FOLDER := .deps
DEPS := $(addprefix $(DEP_FOLDER)/, $(SRC:$(LANG)=.d))

END := \033[0m
BOLD := \033[1m
RED := \033[31m
GREEN := \033[32m
CYAN := \033[36m

SFML_LIBS := -lsfml-graphics -lsfml-window -lsfml-system

OP_FLAGS := -Ofast -fassociative-math -ffast-math
CFLAGS := -std=c++20 -I $(HEADP) -Wall -Wextra $(OP_FLAGS)
ifeq ($(MAKECMDGOALS), trace)
    CFLAGS += -pg
endif

MAKEFLAGS += --no-print-directory --silent

SAY := $(BOLD)[$(CYAN)壺$(END)$(BOLD)]:

all: $(NAME)
.PHONY: all

trace: all
.PHONY: trace

start_compile:
	printf "$(SAY) Praise for the almighty $(CYAN)binary$(END)$(BOLD) !$(END)\n"
.PHONY: start_compile

$(NAME): start_compile $(OBJ)
	$(CC) -fuse-ld=lld -o $(NAME) -I $(HEADP) $(OBJ) $(CFLAGS) -lpthread -ltbb $(SFML_LIBS)
	printf "$(SAY) Ameno ! $(CYAN)$(NAME)$(END)$(BOLD) is among us !$(END)\n"

-include $(DEPS)

format:
	find . -regex '.*\.\(cpp\|hpp\)' -exec clang-format -style=file -i {} \;
.PHONY: format

$(OBJ): | $(OBJ_FOLDER) $(DEP_FOLDER)

$(OBJ):$(OBJ_FOLDER)/%.o: %$(LANG)
	$(CC) $(CFLAGS) -c -o $@ $< -MMD -MF $(DEP_FOLDER)/$*.d	\
	&& printf "$(BOLD)$(CYAN)$< $(END)$(BOLD)has been blessed.$(END)\n"    \
	|| printf "$(BOLD)$(RED) $< $(END)$(BOLD)has been cursed.$(END)\n"

clear:
	printf "$(SAY) Purging heretics files...\n"
	rm -fv source/*~
	rm -fv *~
	rm -fv include/*~
	rm -fv vgcore.*
	printf "$(SAY) Done.$(END)\n\n"
.PHONY: clear

clean:
	printf "$(SAY) ...\n"
	rm -vf $(OBJ)
	rm -f $(DEPS)
	printf "$(SAY) What ? I am just \"rewriting\" the holy book.$(END)\n\n"
.PHONY: clean

fclean: clean
	printf "$(BOLD)Deleting $(NAME)$(END)\n"
	rm -f $(NAME)
	printf "$(SAY) It is a false god. In such, it has been \"deleted\"$(END)\n\n"
.PHONY: fclean

re:
	make clear
	make fclean
	make all
.PHONY: re

hello:
	printf "$(SAY) I am Hu, a wandering believer. My praise are currently to $(NAME).$(END)\n"
	printf "$(SAY) My big brother is named Ri. You may know him ?$(END)\n"
.PHONY: hello

$(OBJ_FOLDER) $(DEP_FOLDER):
	mkdir -p $@
