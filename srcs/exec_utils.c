/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djeon <djeon@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/05 20:00:49 by djeon             #+#    #+#             */
/*   Updated: 2021/07/05 21:56:24 by djeon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void			init_nbf(t_nbf *nbf)
{
	nbf->buf = NULL;
	nbf->env_path = NULL;
	nbf->paths = NULL;
}

int				free_nbf(t_nbf *nbf)
{
	int			i;

	i = -1;
	if (nbf->buf != NULL)
		free(nbf->buf);
	if (nbf->paths != NULL)
	{
		while (nbf->paths[++i] != NULL)
			free(nbf->paths[i]);
		free(nbf->paths);
	}
	return (0);
}

int				check_redirect(t_cmd *cmd_list, int fds[])
{
	int			fd;
	int			right_flag;

	if (redir_err_chk(cmd_list) == -1)
		fd = -1;
	if ((right_flag = redirect_check(cmd_list, &fds)) == -1)
		fd = -1;
	if (cmd_list->pipe_flag == 1 && right_flag == 0)
		fd = fds[1];
	else
		fd = 1;
	return (fd);
}
