/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-hamm <mel-hamm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 15:09:29 by yel-haya          #+#    #+#             */
/*   Updated: 2025/05/27 20:23:58 by mel-hamm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <signal.h>
# include <stdlib.h>
# include <limits.h>
# include <stdio.h>
# include <fcntl.h>
# include <string.h>
# include <errno.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/wait.h>
# include <sys/stat.h>

# define IN_REDIR     2
# define OUT_REDIR    1
# define APPEND_REDIR 3
# define HEREDOC      4

typedef enum s_enums
{
	PIPE = 1000,
	RIGHTPAR = 1001,
	LEFTPAR = 1002,
	OR = 1004,
	RE_IN = 1005,
	RE_OUT = 1006,
	DRE_IN = 1007,
	DRE_OUT = 1008,
	DOUBLE_AND = 1010,
	dollar = 1011,
	DOUBLE_QUOTES = 1013,
	SINGLE_QUOTES = 1014,
	ERROR_CODE = 69,
	FLAGS = 1,
}	t_enums;

typedef struct s_quotes
{
	char			*arg;
	int				len;
	struct s_quotes	*next;
}	t_quotes;

typedef struct s_gc
{
	void		*ptr;
	struct s_gc	*next;
}	t_gc;

typedef struct s_env
{
	char			*full_env;
	char			*variable;
	char			*content;
	struct s_env	*next;
}	t_env;

typedef struct s_heredoc_fds
{
	char					*fd;
	struct s_heredoc_fds	*next;
}	t_heredoc_fds;

typedef struct s_token
{
	char			*cmd;
	t_enums			op;
	int				got_expanded;
	int				do_not_expand;
	int				quotes_existed;
	struct s_token	*prec;
	char			*quotes_flag;
	struct s_token	*next;
}	t_token;

typedef struct s_redir
{
	char			*file;
	int				type;
	int				fd;
	int				do_not_expand;
	int				got_expanded;
	struct s_redir	*next;
}	t_redir;

typedef struct s_exec_list
{
	char				*cmd;
	char				**args;
	t_redir				*redir_list;
	int					in;
	int					out;
	int					built_in;
	int					st;
	int					got_expanded;
	int					flag;
	struct s_exec_list	*next;
}	t_exec_list;

typedef struct s_shell
{
	t_env			*envirement;
	t_quotes		*head;
	t_gc			*collector;
	char			**env;
	char			*command;
	t_token			*token;
	char			**heredoc_files;
	t_exec_list		*executing;
	int				exit_status;
}	t_shell;

typedef struct s_pile
{
	char			str;
	struct s_pile	*next;
}	t_pile;

typedef struct s_list
{
	char	*var;
	char	*value;
	char	*sign;
	int		apnd;
}	t_list;

// signals functions
void		signals_exit_status(int ac, t_shell *ms);
void		signals_ignore(t_shell *minishell);
void		main_signals(t_shell *minishell);
void		heredoc_signals(t_shell *minishell);
void		another_shell_signals(t_shell *minishell);
void		main_sigint_func(int sig);
void		check_heredoc_limit(t_shell *mini);

//expanding funcs
int			check_expand(t_shell *ms);
char		**ftt_split_expand(t_shell *minishell, char const *s);
void		update_quotes_flag(t_shell *ms, t_token *c, \
	size_t start_pos, size_t end_pos);
void		expanding_helper(t_shell *shell, t_token *c);
void		setting_flag(t_shell *ms, t_token *curr);
int			inside_quotes(char *str);
int			expanding_input(t_shell *minishell);
int			normal_expanding(size_t *i, t_token *curr, t_shell *shell);
void		exit_status_case(t_shell *shell, t_token *curr, int n);
int			expand_work(t_token *curr, t_shell *shell);
int			is_valid(char c, int ac);
char		*get_env_value(t_shell *shell, char *var);
char		*ft_substr(t_shell *ms, \
	char const *s, unsigned int start, size_t len);
int			expand_variable(t_shell *shell, t_token *curr);
t_token		*splitting_check(t_shell *shell, t_token *current);
void		reset_env(t_shell *minishell);
t_token		*expanding_spliting(t_shell *ms, t_token *curr);
int			expanding_input_helper(t_shell *shell);
void		expanding_quotes_cases(t_token *c, int *in_s, int *in_d, size_t i);
int			count_nodes(t_exec_list *exec);
int			count_words2(const char *s);
int			split_words_expand(t_shell *minishell, char **f, const char *s);
//exec list functions
void		create_big_list(t_shell *minishell);
int			calc_arg(t_token **token_courant);
void		big_list_add_back(t_shell *minishell, t_exec_list *new);
void		init_redir_list(t_shell *minishell, \
	t_exec_list *exec_courant, t_token *token_courant);
void		rebuilding_cmd(t_shell *ms, t_token *curr, t_quotes *head);

t_quotes	*add_to_quotes_list(t_shell *minishell, \
	char *str, size_t i, int count);
void		parsing(char **env);
void		reset_env_helper(t_shell *minishell, int word_len, int k);
int			redir_handler(t_shell *minishell, \
	char *command, int i, t_token *new);
t_token		*init_cmd(char *command, int *i, t_shell *minishell);
t_env		*new_lst(t_shell *minishell, \
	char *env, char *variable, char *content);
t_token		*init_token(t_shell *minishell, char *command, int *i);
int			tokenize_input(char *command, t_shell *minishell);
int			check_syntax(t_shell *minishell);
int			command_list(t_shell *minishell);
void		strip_out_quotes(t_shell *minishell);
int			set_cmd(t_shell *minishell, char *command, t_token *new, int i);
void		execute_input(t_shell *minishell, char **env);
int			only_dollar(t_token *str);
int			quotes_exist(char *str);
char		*ft_strndup(const char *s1, size_t n);
char		*ftt_strndup(const char *s1, size_t n, t_shell *ms);
void		free_and_flee(t_shell *minishell, int ac);
int			count_words(const char *s, char c);
void		*free_gc(t_gc **collector);
int			check_quotes(t_shell *minishell);
t_env		*new_lst_no_env(t_shell *minishell, char *variable, char *content);

int			cd_(t_shell *minishell);
int			echo_(t_shell *minishell);
int			exit_(t_shell *minishell);
int			export_(t_shell *minishell);
int			pwd_(t_shell *minishell);
int			unset_(t_shell *minishell);
int			execute_pipeline(t_shell *ms);

int			handle_redirections(t_shell *ms, t_exec_list *cmd);
void		close_pipes(int p_fd[2], t_exec_list *next);
char		*fetch_env(char *variable, t_shell *minishell);
int			set_env(char *var, char *value, t_shell *minishell);
int			is_valid_identifier(char *name);
int			handle_heredoc(t_shell *ms, char *delimiter, int f, int status);
int			exec_(t_shell *ms);
int			exec_external(t_exec_list *cmd, t_shell *ms);
int			execute_cmd(t_exec_list *cmd, t_shell *ms);
int			env_(t_shell *minishell);
int			is_builtin(char *cmd);
int			dup_for_built_in(t_shell *ms);
char		*get_target_dir(char **args, t_shell *ms);
int			assign_or_append(t_list *list, int appnd, t_shell *ms);
void		*ft_alloc(size_t size, t_gc **collector, int ac);
char		*get_cmd_path(char *cmd, char **env, t_shell *ms);
int			handle_export_arg(char *arg, t_shell *ms);
int			cmd_err(t_shell *ms, char *cmd, char *msg, int status);
char		**get_echo_args(t_shell *ms);
char		**get_cd_args(t_shell *ms);
char		**get_env_args(t_shell *ms);
void		close_unused_fds(t_exec_list *cmd);
char		**get_export_args(t_shell *ms);
char		**get_exit_args(t_shell *ms);
void		closing(int first, int second);
void		cleanup_heredoc_files(t_shell *shell);
char		*expanding_heredoc(t_shell *shell, char *line);
int			ft_strcmp(char *s1, char *s2);
int			check_failure(int status, int *p_fd, t_exec_list *current, \
	t_shell *ms);
char		*ft_strchr(const char *s, int c);
char		*ftt_strdup(const char *s1, t_shell *ms);
char		*ftt_itoa(t_shell *minishell, int n);
char		**ftt_split(t_shell *minishell, char const *s, char c);
char		*ftt_strjoin(t_shell *minishell, char const *s1, char const *s2);
long		ft_strtol(const char *str, char **endptr);

#endif
