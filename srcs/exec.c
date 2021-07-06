/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seuyu <seuyu@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/16 18:06:01 by djeon             #+#    #+#             */
/*   Updated: 2021/07/06 11:03:34 by djeon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int				non_builtin_exec(t_cmd *cmd_list, char *argv[], char **envp,
		int fds[])
{
	int			status;
	pid_t		pid;
	int			i;

	i = 0;
	argv[0] = cmd_list->path;
	while (cmd_list->cmdline[++i].cmd != NULL
			&& cmd_list->cmdline[i].redir_flag == 0)
		argv[i] = cmd_list->cmdline[i].cmd;
	argv[i] = NULL;
	if ((pid = fork()) < -1)
		return (-1);
	if (pid == 0)
	{
		if (cmd_list->pipe_flag == 1)
			dup2(fds[1], 1);
		if (execve(cmd_list->path, argv, envp) == -1)
			return (-1);
	}
	else if (pid != 0)
	{
		waitpid(pid, &status, 0);
		g_exit_status = status >> 8;
	}
	return (0);
}

int				compare_env_path(t_cmd *cmd_list, t_nbf *nbf, t_arg st_arg, int fds[])
{
	int			i;
	int			flag;

	i = 0;
	flag = 0;
	if (((nbf->env_path = get_env_value("PATH", st_arg.envp)) == NULL) ||
			((nbf->paths = ft_split(nbf->env_path, ':')) == NULL))
		return (-1);
	while (nbf->paths[++i] != NULL)
	{
		if ((cmd_list->path =
				strjoin_path(nbf->paths[i], cmd_list->cmdline[0].cmd)) == 0)
			return (-1);
		if (cmd_list->cmdline[0].cmd[0] != '\0'
				&& stat(cmd_list->path, nbf->buf) == 0)
		{
			if (non_builtin_exec(cmd_list, st_arg.argv, st_arg.envp, fds) == -1)
				return (-1);
			flag = 1;
		}
		free(cmd_list->path);
		if (flag == 1)
			break ;
	}
	free_nbf(nbf);
	return (flag);
}

int				non_builtin(t_cmd *cmd_list, t_arg st_arg, int fds[])
{
	t_nbf		nbf;
	int			i;
	int			flag;

	i = -1;
	flag = 0;
	init_nbf(&nbf);
	if (!(nbf.buf = (struct stat*)malloc(sizeof(struct stat))))
		return (0);
	if (cmd_list->cmdline[0].cmd[0] != '\0' &&
			stat(cmd_list->cmdline[0].cmd, nbf.buf) == 0)
	{
		cmd_list->path = cmd_list->cmdline[0].cmd;
		if ((non_builtin_exec(cmd_list, st_arg.argv, st_arg.envp, fds)) == -1)
			return (free_nbf(&nbf));
		flag = 1;
	}
	else
		if ((flag = compare_env_path(cmd_list, &nbf, st_arg, fds)) == -1)
			return (free_nbf(&nbf));
	if (flag == 1 || cmd_list->cmdline[0].redir_flag == 1)
		return (1);
	cmd_list->err_manage.errcode = 1;
	return (0);
}

int				exec_function(t_cmd *cmd_list, char *argv[], char **envp[],
		int fds[])
{
	int			fd;
	t_arg		st_arg;

	st_arg.argv = argv;
	st_arg.envp = *envp;
	if ((fd = check_redirect(cmd_list, fds)) == -1)
		return (-1);
	if (ft_strncmp("pwd", cmd_list->cmdline[0].cmd, 4) == 0)
		return (ft_pwd(fd));
	else if (ft_strncmp("cd", cmd_list->cmdline[0].cmd, 3) == 0)
		return (ft_cd(cmd_list));
	else if (ft_strncmp("exit", cmd_list->cmdline[0].cmd, 5) == 0)
		return (ft_exit(cmd_list));
	else if (ft_strncmp("env", cmd_list->cmdline[0].cmd, 4) == 0)
		return (ft_env(*envp, fd));
	else if (ft_strncmp("export", cmd_list->cmdline[0].cmd, 7) == 0)
		return (ft_export(cmd_list, envp, fd));
	else if (ft_strncmp("echo", cmd_list->cmdline[0].cmd, 5) == 0)
		return (ft_echo(cmd_list, fd));
	else if (ft_strncmp("unset", cmd_list->cmdline[0].cmd, 6) == 0)
		return (ft_unset(cmd_list, *envp));
	else if (non_builtin(cmd_list, st_arg, fds) == 0)
		return (-1);
	return (0);
}

int				exec(t_cmd *cmd_list, char *argv[], char **envp[])
{
	int			fds[2];
	int			status;
	pid_t		pid;
	pid_t		wpid;
	int			tmp;

	pipe(fds);
	if ((tmp = exec_function(cmd_list, argv, envp, fds)) == -1)
		print_errstr(cmd_list);
	else if (tmp == 1)
		g_exit_status = 0;
	dup2(100, STDOUT);
	dup2(101, STDIN);
	if (cmd_list->pipe_flag == 1)
	{
		if ((pid = fork()) < 0)
		{
			cmd_list->err_manage.errcode = 1;
			print_errstr(cmd_list);
			return (g_exit_status);
		}
	}
	else
		return (g_exit_status);
	if (pid == 0)
	{
		dup2(fds[0], 0);
		close(fds[0]);
		close(fds[1]);
		g_exit_status = exec(cmd_list->next, argv, envp);
		exit(g_exit_status);
	}
	else
	{
		close(fds[1]);
		close(fds[0]);
		wpid = waitpid(pid, &status, 0);
		g_exit_status = status >> 8;
		return (g_exit_status);
	}
}
