# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: dande-je <dande-je@student.42sp.org.br>    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Updated: 2024/11/20 23:14:40 by maurodri         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#******************************************************************************#
#                                   COLOR                                      #
#******************************************************************************#

RED                             := \033[0;31m
GREEN                           := \033[0;32m
YELLOW                          := \033[0;33m
PURPLE                          := \033[0;35m
CYAN                            := \033[0;36m
RESET                           := \033[0m

#******************************************************************************#
#                                   PATH                                       #
#******************************************************************************#

SRCS_DIR                        := src/
SRCS_INTERNAL_DIR               := src/internal/
SRCS_ENV_DIR                    := $(SRCS_INTERNAL_DIR)env/
SRCS_ENV_INTERNAL_DIR           := $(SRCS_ENV_DIR)env_internal/
SRCS_REPL_DIR                   := $(SRCS_INTERNAL_DIR)repl/
SRCS_SIGNAL_DIR                 := $(SRCS_INTERNAL_DIR)signal/
SRCS_SIGNAL_INTERNAL_DIR        := $(SRCS_SIGNAL_DIR)signal_internal/
SRCS_HISTORY_DIR                := $(SRCS_REPL_DIR)history/
SRCS_SHELL_DIR                  := $(SRCS_REPL_DIR)shell/
SRCS_COMMAND_DIR                := $(SRCS_SHELL_DIR)command/
SRCS_PARSE_DIR                  := $(SRCS_SHELL_DIR)parse/
SRCS_PARSE_INTERNAL_DIR         := $(SRCS_PARSE_DIR)parse_internal/
SRCS_RUNNER_DIR                 := $(SRCS_SHELL_DIR)runner/
SRCS_EXPAND_DIR                 := $(SRCS_RUNNER_DIR)expand/
SRCS_BUILTINS_DIR               := $(SRCS_RUNNER_DIR)builtins/
SRCS_TOKEN_DIR                  := $(SRCS_SHELL_DIR)token/
SRCS_TOKEN_INTERNAL_DIR         := $(SRCS_TOKEN_DIR)token_internal/
SRCS_TERMINAL_DIR               := $(SRCS_REPL_DIR)terminal/
SRCS_BONUS_DIR                  := bonus/
SRCS_INTERNAL_BONUS_DIR         := bonus/internal_bonus/
SRCS_ENV_BONUS_DIR              := $(SRCS_INTERNAL_BONUS_DIR)env/
SRCS_ENV_INTERNAL_BONUS_DIR     := $(SRCS_ENV_BONUS_DIR)env_internal/
SRCS_REPL_BONUS_DIR             := $(SRCS_INTERNAL_BONUS_DIR)repl/
SRCS_SIGNAL_BONUS_DIR           := $(SRCS_INTERNAL_BONUS_DIR)signal/
SRCS_SIGNAL_INTERNAL_BONUS_DIR  := $(SRCS_SIGNAL_BONUS_DIR)signal_internal/
SRCS_HISTORY_BONUS_DIR          := $(SRCS_REPL_BONUS_DIR)history/
SRCS_SHELL_BONUS_DIR            := $(SRCS_REPL_BONUS_DIR)shell/
SRCS_COMMAND_BONUS_DIR          := $(SRCS_SHELL_BONUS_DIR)command/
SRCS_PARSE_BONUS_DIR            := $(SRCS_SHELL_BONUS_DIR)parse/
SRCS_PARSE_INTERNAL_BONUS_DIR   := $(SRCS_PARSE_BONUS_DIR)parse_internal/
SRCS_RUNNER_BONUS_DIR           := $(SRCS_SHELL_BONUS_DIR)runner/
SRCS_EXPAND_BONUS_DIR           := $(SRCS_RUNNER_BONUS_DIR)expand/
SRCS_BUILTINS_BONUS_DIR         := $(SRCS_RUNNER_BONUS_DIR)builtins/
SRCS_TOKEN_BONUS_DIR            := $(SRCS_SHELL_BONUS_DIR)token/
SRCS_TOKEN_INTERNAL_BONUS_DIR   := $(SRCS_TOKEN_BONUS_DIR)token_internal/
SRCS_TERMINAL_BONUS_DIR         := $(SRCS_REPL_BONUS_DIR)terminal/
INCS                            := src/ bonus/ lib/libftx/includes/
BUILD_DIR                       := build/
LIBFTX_DIR                      := lib/libftx/

#******************************************************************************#
#                                  COMMANDS                                    #
#******************************************************************************#

RM                              := rm -rf
MKDIR                           := mkdir -p
MAKEFLAGS                       += --no-print-directory
SLEEP                           := sleep 0.01

#******************************************************************************#
#                                   FILES                                      #
#******************************************************************************#

LIBFTX                          := $(addprefix $(LIBFTX_DIR), libft.a)
LIBS                            := ./lib/libftx/libft.a

NAME                            = minishell
NAME_BONUS                      = minishell_bonus

SRCS_FILES                      += $(addprefix $(SRCS_DIR), main.c)
SRCS_FILES                      += $(addprefix $(SRCS_ENV_DIR), env.c \
								   env_properties.c \
								   envp.c)
SRCS_FILES                      += $(addprefix $(SRCS_ENV_INTERNAL_DIR), env_build.c \
								   env_destroy.c \
								   env_manage.c \
								   env_path.c \
								   env_update.c)
SRCS_FILES                      += $(addprefix $(SRCS_REPL_DIR), repl.c)
SRCS_FILES                      += $(addprefix $(SRCS_SIGNAL_DIR), signal.c)
SRCS_FILES                      += $(addprefix $(SRCS_SIGNAL_INTERNAL_DIR), signal_handler.c \
								   signal_heredoc.c \
								   signal_initializer.c)
SRCS_FILES                      += $(addprefix $(SRCS_TERMINAL_DIR), terminal.c)
SRCS_FILES                      += $(addprefix $(SRCS_COMMAND_DIR), command.c \
								   command_build.c \
								   command_build_paren.c \
								   command_build_panic.c \
								   command_build_util.c \
								   command_invalid.c \
								   command_simple.c \
								   command_simple_fill.c \
								   command_pipe.c \
								   command_and.c \
								   command_or.c \
								   command_paren.c \
								   command_eof.c \
								   command_util.c \
								   io_handler.c \
								   io_handler_set.c \
								   io_handlers.c\
								   io_handlers_add.c \
								   io_handler_heredoc.c \
								   io_handlers_heredoc.c)
SRCS_FILES                      += $(addprefix $(SRCS_HISTORY_DIR), history.c)
SRCS_FILES                      += $(addprefix $(SRCS_PARSE_DIR), parse.c)
SRCS_FILES                      += $(addprefix $(SRCS_PARSE_INTERNAL_DIR), parse_operator.c \
								   parse_word.c)
SRCS_FILES                      += $(addprefix $(SRCS_RUNNER_DIR), runner.c \
								   runner_cmd.c \
								   runner_cmd_simple.c \
								   runner_cmd_pipe.c \
								   runner_cmd_or.c \
								   runner_cmd_and.c \
								   runner_cmd_paren.c \
								   runner_cmd_simple_execve.c \
								   runner_heredoc.c \
								   runner_data.c)
SRCS_FILES                      += $(addprefix $(SRCS_EXPAND_DIR), expand.c \
								  expand_util.c \
								  expand_icompare_str.c \
								  expand_split_str.c \
								  expand_argv.c \
								  expand_str.c \
								  expand_str_star.c \
								  expand_io.c)
SRCS_FILES                      += $(addprefix $(SRCS_BUILTINS_DIR), builtins.c \
								   builtin_util.c \
								   cd.c \
								   echo.c \
								   env.c \
								   exit.c \
								   export.c \
								   pwd.c \
								   unset.c)
SRCS_FILES                      += $(addprefix $(SRCS_SHELL_DIR), shell.c)
SRCS_FILES                      += $(addprefix $(SRCS_TOKEN_DIR), token.c \
								  token_type.c)
SRCS_FILES                      += $(addprefix $(SRCS_TOKEN_INTERNAL_DIR), token_build.c \
								   token_classify.c \
								   token_destroy.c)

SRCS_BONUS_FILES                += $(addprefix $(SRCS_BONUS_DIR), main.c)
SRCS_BONUS_FILES                += $(addprefix $(SRCS_ENV_BONUS_DIR), env.c \
								   env_properties.c \
								   envp.c)
SRCS_BONUS_FILES                += $(addprefix $(SRCS_ENV_INTERNAL_BONUS_DIR), env_build.c \
								   env_destroy.c \
								   env_manage.c \
								   env_path.c \
								   env_update.c)
SRCS_BONUS_FILES                += $(addprefix $(SRCS_REPL_BONUS_DIR), repl.c)
SRCS_BONUS_FILES                += $(addprefix $(SRCS_SIGNAL_BONUS_DIR), signal.c)
SRCS_BONUS_FILES                += $(addprefix $(SRCS_SIGNAL_INTERNAL_BONUS_DIR), signal_handler.c \
								   signal_heredoc.c \
								   signal_initializer.c)
SRCS_BONUS_FILES                += $(addprefix $(SRCS_TERMINAL_BONUS_DIR), terminal.c)
SRCS_BONUS_FILES                += $(addprefix $(SRCS_COMMAND_BONUS_DIR), command.c \
								   command_build.c \
								   command_build_paren.c \
								   command_build_panic.c \
								   command_build_util.c \
								   command_invalid.c \
								   command_simple.c \
								   command_simple_fill.c \
								   command_pipe.c \
								   command_and.c \
								   command_or.c \
								   command_paren.c \
								   command_eof.c \
								   command_util.c \
								   io_handler.c \
								   io_handler_set.c \
								   io_handlers.c\
								   io_handlers_add.c \
								   io_handler_heredoc.c \
								   io_handlers_heredoc.c)
SRCS_BONUS_FILES                += $(addprefix $(SRCS_HISTORY_BONUS_DIR), history.c)
SRCS_BONUS_FILES                += $(addprefix $(SRCS_PARSE_BONUS_DIR), parse.c)
SRCS_BONUS_FILES                += $(addprefix $(SRCS_PARSE_INTERNAL_BONUS_DIR), parse_operator.c \
								   parse_word.c)
SRCS_BONUS_FILES                += $(addprefix $(SRCS_RUNNER_BONUS_DIR), runner.c \
								   runner_cmd.c \
								   runner_cmd_simple.c \
								   runner_cmd_pipe.c \
								   runner_cmd_or.c \
								   runner_cmd_and.c \
								   runner_cmd_paren.c \
								   runner_cmd_simple_execve.c \
								   runner_heredoc.c \
								   runner_data.c)
SRCS_BONUS_FILES                 += $(addprefix $(SRCS_EXPAND_BONUS_DIR), expand.c \
								  expand_util.c \
								  expand_icompare_str.c \
								  expand_split_str.c \
								  expand_argv.c \
								  expand_str.c \
								  expand_str_star.c \
								  expand_io.c)
SRCS_BONUS_FILES                += $(addprefix $(SRCS_BUILTINS_BONUS_DIR), builtins.c \
								   builtin_util.c \
								   cd.c \
								   echo.c \
								   env.c \
								   exit.c \
								   export.c \
								   pwd.c \
								   unset.c)
SRCS_BONUS_FILES                += $(addprefix $(SRCS_SHELL_BONUS_DIR), shell.c)
SRCS_BONUS_FILES                += $(addprefix $(SRCS_TOKEN_BONUS_DIR), token.c \
								  token_type.c)
SRCS_BONUS_FILES                += $(addprefix $(SRCS_TOKEN_INTERNAL_BONUS_DIR), token_build.c \
								   token_classify.c \
								   token_destroy.c)

OBJS                            += $(SRCS_FILES:%.c=$(BUILD_DIR)%.o)
OBJS_BONUS                      += $(SRCS_BONUS_FILES:%.c=$(BUILD_DIR)%.o)

DEPS                            += $(OBJS:.o=.d)

#******************************************************************************#
#                               OUTPUTS MESSAGES                               #
#******************************************************************************#

COUNT                           = 0
OBJS_COUNT                      = 0
MATH                            = 0
CLEAN_MESSAGE                   := Minishell objects deleted
FCLEAN_MESSAGE                  := Minishell deleted
EXE_MESSAGE                     = $(RESET)[100%%] $(GREEN)Built target minishell
EXE_BONUS_MESSAGE               = [100%%] $(GREEN)Built target minishell_bonus
COMP_MESSAGE                    = Building C object
COMP_BONUS_MESSAGE              = $(CYAN)[BONUS]$(RESET) $(YELLOW)Building C object

#******************************************************************************#
#                               COMPILATION                                    #
#******************************************************************************#

CC                             := cc
CFLAGS                         = -Wall -Wextra -Werror -Ofast
CPPFLAGS                       := $(addprefix -I,$(INCS)) -MMD -MP
DFLAGS                         := -Wall -Wextra -Werror -g3
LFLAGS                         := -march=native
LDFLAGS                        := $(addprefix -L,$(dir $(LIBS)))
LDLIBS                         := -lft -ldl -lreadline
COMPILE_OBJS                   = $(CC) $(CFLAGS) $(LFLAGS) $(CPPFLAGS) -c $< -o $@
COMPILE_EXE                    = $(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) $(LDLIBS) -o $(NAME)

#******************************************************************************#
#                                   DEFINE                                     #
#******************************************************************************#

ifdef WITH_DEBUG
	CFLAGS                     = $(DFLAGS)
endif

ifdef WITH_BONUS
	NAME                       = $(NAME_BONUS)
	OBJS                       = $(OBJS_BONUS)
	COMP_MESSAGE               = $(COMP_BONUS_MESSAGE)
	EXE_MESSAGE                = $(EXE_BONUS_MESSAGE)
endif

#******************************************************************************#
#                                  FUNCTION                                    #
#******************************************************************************#

define create_dir
	$(MKDIR) $(dir $@)
endef

define build_libftx
	printf "$(PURPLE)Building library Libftx\n$(RESET)"
	git submodule update --init --remote >/dev/null 2>&1 || true
	git submodule foreach -q \
		'branch="$(git config -f $toplevel/.gitmodules submodule.libftx)"; \
		git pull origin master; \
		git fetch; \
		git checkout v$(LIBFTX_VERSION)' \
		>/dev/null 2>&1 || true
	$(SLEEP)
	$(MAKE) -C $(LIBFTX_DIR)
endef

define comp_objs
	$(eval COUNT=$(shell expr $(COUNT) + 1))
	$(COMPILE_OBJS)
	$(eval MATH=$(shell expr "$(COUNT)" \* 100 \/ "$(OBJS_COUNT)"))
	$(eval MATH=$(shell if [ $(COUNT) -lt 1 ]; then echo $(shell expr "$(MATH)" + 100) ; else echo $(MATH) ; fi))
	printf "[%3d%%] $(YELLOW)$(COMP_MESSAGE) $@ \r$(RESET)\n" $$(echo $(MATH))
endef

define bonus
	$(MAKE) WITH_BONUS=TRUE
endef

define comp_exe
	$(COMPILE_EXE)
	$(SLEEP)
	printf "$(EXE_MESSAGE)\n$(RESET)"
endef

define clean
	$(RM) $(BUILD_DIR)
	$(MAKE) fclean -C $(LIBFTX_DIR)
	$(SLEEP)
	printf "$(RED)$(CLEAN_MESSAGE)$(RESET)\n"
endef

define fclean
	$(RM) $(NAME) $(NAME_BONUS)
	$(SLEEP)
	printf "$(RED)$(FCLEAN_MESSAGE)$(RESET)\n"
endef

define debug
	$(MAKE) WITH_DEBUG=TRUE
endef

define reset_count
	$(eval COUNT=$(1))
	$(eval OBJS_COUNT=$(words $(SRCS_FILES)))
endef

#******************************************************************************#
#                                   TARGETS                                    #
#******************************************************************************#

all: $(NAME)

$(BUILD_DIR)%.o: %.c
	$(call create_dir)
	$(call comp_objs)

$(NAME): $(LIBFTX) $(call reset_count, -$(words $(OBJS))) $(OBJS)
	$(call comp_exe)

$(LIBFTX):
	$(call build_libftx)

bonus:
	$(call bonus)

clean:
	$(call clean)

fclean: clean
	$(call fclean)

re: fclean all

debug:
	$(call debug)

.PHONY: all clean fclean re debug etags bonus
.DEFAULT_GOAL := all
.SILENT:

-include $(DEPS)
