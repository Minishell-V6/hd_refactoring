/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: seojeong <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/14 19:52:35 by djeon             #+#    #+#             */
/*   Updated: 2021/07/05 20:30:05 by djeon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int				g_exit_status = 0;

int				main(int argc, char *argv[], char *envp[])
{
	t_cmd		*cmd_list;
	char		*line;
	char		**cpenv;

	dup2(STDIN, 100);
	dup2(STDOUT, 101);
	argc = 1;
	cpenv = copy_envp(envp);
	set_signal();
	while ((line = readline("minishell $ ")))
	{
		if (*line != '\0' && !check_whitespace(line))
		{
			add_history(line);
			parse(&cmd_list, line, cpenv);
			g_exit_status = exec(cmd_list, argv, &cpenv);
			free_list(cmd_list);
		}
		free(line);
	}
	ft_putstr_fd("\x1b[1A", STDOUT);
	ft_putstr_fd("\033[12C", STDOUT);
	ft_putstr_fd("exit\n", STDOUT);
	return (0);
}
