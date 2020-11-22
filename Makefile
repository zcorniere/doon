##
## A Makefile with faith
## Hu 2.0
##

NAME := doon
HEADP := ./include/

SRC_FOLDER := source
OBJ_FOLDER := .object
TEST_FOLDER := tests

CC := g++
LANG := .cpp
VPATH := $(SRC_FOLDER)
SRC := $(notdir $(shell find $(SRC_FOLDER) -name '*$(LANG)'))
OBJ := $(addprefix $(OBJ_FOLDER)/,$(SRC:$(LANG)=.o))
OBJM := $(filter-out $(OBJ_FOLDER)/main.o, $(OBJ))

VPATH += $(TEST_FOLDER)
TEST_SRC := $(notdir $(shell find $(TEST_FOLDER) -name '*$(LANG)'))
TEST_OBJ := $(addprefix $(OBJ_FOLDER)/, $(TEST_SRC:$(LANG)=.o))
TFLAGS := -lcriterion

DEP_FOLDER := .deps
DEPS := $(addprefix $(DEP_FOLDER), $(SRC:$(LANG)=.d))
TEST_DEPS := $(addprefix $(DEP_FOLDER), $(TEST_SRC:$(LANG)=.d))

END := \033[0m
BOLD := \033[1m
RED := \033[31m
GREEN := \033[32m
CYAN := \033[36m

SFML_LIBS := -lsfml-graphics -lsfml-window -lsfml-system
CFLAGS := -std=c++20 -O2 -I $(HEADP) -Wall -Wextra -Werror $(SFML_LIBS)

MAKEFLAGS += --no-print-directory --silent

SAY := $(BOLD)[$(CYAN)壺$(END)$(BOLD)]:

all: $(NAME)
.PHONY: all

-include $(DEPS) $(TEST_DEPS)

start_compile:
	printf "$(SAY) Praise for the almighty $(CYAN)binary$(END)$(BOLD) !$(END)\n"
.PHONY: start_compile

$(NAME): start_compile $(OBJ)
	$(CC) -o $(NAME) -I $(HEADP) $(OBJ) $(CFLAGS)
	printf "$(SAY) Ameno ! $(CYAN)$(NAME)$(END)$(BOLD) is among us !$(END)\n"

tests_run: $(OBJ) $(TEST_OBJ)
	printf "$(SAY) Are you doubting my faith ?$(END)\n"
	$(CC) -o unit_tests $(OBJM) $(TEST_OBJ) $(TFLAGS) $(CFLAGS)
	./unit_tests -j4 $(VERBOSE)
.PHONY: tests_run

unit_tests: $(OBJM) $(TEST_OBJ)
	$(CC) -o unit_tests $(OBJM) $(TEST_OBJ) $(TFLAGS) $(CFLAGS)
.PHONY: unit_tests

$(OBJ): | $(OBJ_FOLDER) $(DEP_FOLDER)

$(OBJ):$(OBJ_FOLDER)/%.o: %$(LANG)
	$(CC) $(CFLAGS) -c -o $@ $<	\
	&& printf "$(BOLD)$(CYAN)$< $(END)$(BOLD)has been blessed.$(END)\n"    \
	|| printf "$(BOLD)$(RED) $< $(END)$(BOLD)has been cursed.$(END)\n"
	$(CC) $(CFLAGS) -MM -MP -MT $@ $< > $(DEP_FOLDER)/$*.d

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

fclean: tclean clean
	printf "$(BOLD)Deleting $(NAME)$(END)\n"
	rm -f $(NAME)
	printf "$(SAY) It is a false god. In such, it has been \"deleted\"$(END)\n\n"
.PHONY: fclean

tclean:
	printf "$(BOLD)Deleting your skepticism.\n"
	rm -f $(TEST_DEPS)
	rm -vf $(TEST_OBJ)
	rm -vf tests_run
	printf "$(CYAN)* * * * * SKEPTICISM REMOVED * * * * *$(END)\n\n"
.PHONY: tclean

re:
	make clear
	make fclean
	make all
.PHONY: re

$(TEST_OBJ): $(OBJ_FOLDER)/%.o: %$(LANG)
	$(CC) $(CFLAGS) $(TFLAGS) -c -o $@ $< \
	&& printf "$(BOLD)$(CYAN)$< $(END)$(BOLD)is ready.$(END)\n"    \
	|| printf "$(BOLD)$(RED) $< $(END)$(BOLD)is not ready.$(END)\n"
	$(CC) $(CFLAGS) -MM -MP -MT $@ $< > $(DEP_FOLDER)/$*.d

hello:
	printf "$(SAY) I am Hu, a wandering believer. My praise are currently to $(NAME).$(END)\n"
	printf "$(SAY) My big brother is named Ri. You may know him ?$(END)\n"
.PHONY: hello

$(OBJ_FOLDER) $(DEP_FOLDER):
	mkdir -p $@
