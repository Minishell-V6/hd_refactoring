/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djeon <djeon@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/30 16:27:21 by djeon             #+#    #+#             */
/*   Updated: 2021/07/06 15:41:04 by djeon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/unset.h"

int				ft_find_envkey(char *key, char **envp)
{
	int			i;
	size_t		key_len;

	i = 0;
	key_len = ft_strlen(key);
	while (envp[i])
	{
		if (ft_strncmp(key, envp[i], key_len) == 0 && envp[i][key_len] == '=')
			return (i);
		i++;
	}
	return (-1);
}

void			ft_delete_env(int tgt_idx, char **envp)
{
	free(envp[tgt_idx]);
	while (envp[tgt_idx])
	{
		envp[tgt_idx] = envp[tgt_idx + 1];
		tgt_idx++;
	}
}

int				ft_valid_key(char *key)
{
	int			i;

	i = 0;
	if (ft_strlen(key) == 0)
		return (0);
	while (key[i])
	{
		if (ft_isdigit(key[0]))
			return (0);
		if (!ft_isalnum(key[i]) && key[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

int				ft_unset(t_cmd *cmd_list, char **envp)
{
	int			i;
	int			tgt_idx;

	i = 1;
	while (cmd_list->cmdline[i].cmd && cmd_list->cmdline[i].redir_flag == 0)
	{
		if (ft_valid_key(cmd_list->cmdline[i].cmd))
		{
			tgt_idx = ft_find_envkey(cmd_list->cmdline[i].cmd, envp);
			if (tgt_idx > -1)
				ft_delete_env(tgt_idx, envp);
		}
		else
			cmd_list->err_manage.errcode = 6;
		i++;
	}
	if (cmd_list->err_manage.errcode == 6)
		return (-1);
	return (1);
}
