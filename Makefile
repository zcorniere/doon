##
## A Makefile with faith
## Hu 2.0
##

NAME := doon

ENGINE := ./engine/

HEADP := include/

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
OPTIONAL_LIBS := -lpthread -ltbb

OP_FLAGS := -Ofast -fassociative-math -ffast-math
CFLAGS := -std=c++20 -I $(ENGINE)$(HEADP) -I $(HEADP) -Wall -Wextra $(OP_FLAGS)
ifeq ($(MAKECMDGOALS), trace)
    CFLAGS += -pg
endif

MAKEFLAGS += --no-print-directory --silent

SAY := $(BOLD)[$(CYAN)壺$(END)$(BOLD)]:

all:
	make engine
	make $(NAME)
.PHONY: all

compile_command: all
	sed -e '1s/^/[\'$$'\n''/' -e '$$s/,$$/\'$$'\n'']/' $(OBJ_FOLDER)/*.json > compile_commands.json
.PHONY: compile_command

trace: all
.PHONY: trace

start_compile:
	printf "$(SAY) Listen to our apostle $(CYAN)$(NAME)$(END)$(BOLD)!$(END)\n"
.PHONY: start_compile

engine:
	make -C $(ENGINE)
.PHONY: engine

$(NAME): start_compile $(OBJ)
	$(CC) -L $(ENGINE) -lraycaster -fuse-ld=lld -o $(NAME)  $(OBJ) $(CFLAGS) $(OPTIONAL_LIBS) $(SFML_LIBS)
	printf "$(SAY) Praise $(CYAN)$(NAME)$(END)$(BOLD) !$(END)\n"

-include $(DEPS)

format:
	find $(SRC_FOLDER) $(ENGINE) $(HEADP) -regex '.*\.\(cpp\|hpp\)' -exec clang-format -style=file -i {} \;
.PHONY: format

$(OBJ): | $(OBJ_FOLDER) $(DEP_FOLDER)

$(OBJ):$(OBJ_FOLDER)/%.o: %$(LANG)
	$(CC) $(CFLAGS) -c -o $@ $< -MJ $(OBJ_FOLDER)/$*.json -MMD -MF $(DEP_FOLDER)/$*.d	\
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
	make -C $(ENGINE) fclean
	make all
.PHONY: re

$(OBJ_FOLDER) $(DEP_FOLDER):
	mkdir -p $@
