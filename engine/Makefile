NAME := libraycaster.a
HEADP := ./include/

SRC_FOLDER := source
OBJ_FOLDER := ../.object

CC := clang++

LANG := .cpp
VPATH := $(SRC_FOLDER)
SRC := $(notdir $(shell find $(SRC_FOLDER) -name '*$(LANG)'))
OBJ := $(addprefix $(OBJ_FOLDER)/,$(SRC:$(LANG)=.o))

DEP_FOLDER := ../.deps
DEPS := $(addprefix $(DEP_FOLDER)/, $(SRC:$(LANG)=.d))

END := \033[0m
BOLD := \033[1m
RED := \033[31m
GREEN := \033[32m

OP_FLAGS := -Ofast -fassociative-math -ffast-math
CFLAGS := -fPIC -std=c++20 -I $(HEADP) -Wall -Wextra $(OP_FLAGS)

MAKEFLAGS += --no-print-directory --silent

SAY := $(BOLD)[$(GREEN)*$(END)$(BOLD)]:

all: $(NAME)
.PHONY: all

start_compile:
	printf "$(SAY) Praise for the almighty $(GREEN)library$(END)$(BOLD) !$(END)\n"
.PHONY: start_compile

$(NAME): start_compile $(OBJ)
	ar rc $(NAME) $(OBJ)
	printf "$(SAY) Ameno ! $(GREEN)$(NAME)$(END)$(BOLD) is among us !$(END)\n"

-include $(DEPS)

$(OBJ): | $(OBJ_FOLDER) $(DEP_FOLDER)

$(OBJ):$(OBJ_FOLDER)/%.o: %$(LANG)
	$(CC) $(CFLAGS) -c -o $@ $< -MJ $(OBJ_FOLDER)/$*.json -MMD -MF $(DEP_FOLDER)/$*.d	\
	&& printf "$(BOLD)$(GREEN)$<$(END)$(BOLD) has awoken.$(END)\n"    \
	|| printf "$(BOLD)$(RED) $<$(END)$(BOLD) has fall down.$(END)\n"

clean:
	printf "$(SAY) ...\n"
	rm -vf $(OBJ)
	rm -f $(DEPS)
	printf "$(SAY) Huh ? They are heretics, don't worry about them.$(END)\n\n"
.PHONY: clean

fclean: clean
	printf "$(BOLD)Deleting $(NAME)$(END)\n"
	rm -f $(NAME)
	printf "$(SAY) The heretics has been cleansed !$(END)\n\n"
.PHONY: fclean

re:
	make fclean
	make all
.PHONY: re

$(OBJ_FOLDER) $(DEP_FOLDER):
	mkdir -p $@
