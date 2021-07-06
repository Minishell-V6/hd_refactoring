/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djeon <djeon@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/07/06 13:09:31 by djeon             #+#    #+#             */
/*   Updated: 2021/07/06 13:10:34 by djeon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void			str_forward(char *line, int *i)
{
	int j;

	j = 0;
	while (line[(*i) + j])
	{
		line[(*i) + j] = line[(*i) + j + 1];
		j++;
	}
	(*i)--;
}

char			check_quote(char *line)
{
	char		result;
	int			i;

	i = -1;
	result = 0;
	while (line[++i] != '\0')
	{
		if (line[i] == '"' && (result == 0 || result == '"'))
		{
			if (result == '"')
				result = 0;
			else
				result = '"';
			str_forward(line, &i);
		}
		else if (line[i] == '\'' && (result == 0 || result == '\''))
		{
			if (result == '\'')
				result = 0;
			else
				result = '\'';
			str_forward(line, &i);
		}
	}
	return (result);
}
