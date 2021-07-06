/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_split.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sejpark <sejpark@student.42seoul.kr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/07 15:33:09 by sejpark           #+#    #+#             */
/*   Updated: 2021/07/04 17:09:56 by sejpark          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cmd_split.h"

void				put_quote_flag(char const *s, int *quote)
{
	if (*s == '"' && ((*quote) == 0 || (*quote) == '"'))
	{
		if ((*quote) == '"')
			(*quote) = 0;
		else
			(*quote) = '"';
	}
	else if (*s == '\'' && ((*quote) == 0 || (*quote) == '\''))
	{
		if ((*quote) == '\'')
			(*quote) = 0;
		else
			(*quote) = '\'';
	}
}

void				get_cnt(int *redir, int *cnt,
							int *begin, int begin_zero_flag)
{
	if (*redir)
		(*redir) = 0;
	if (begin_zero_flag == 1)
		(*begin) = 0;
	(*cnt)++;
}

void				condition_to_cnt(t_split_flag *f,
					char const *s, const char c, int *cnt)
{
	if (*s == c && f->quote == 0)
		get_cnt(&f->redir, cnt, &f->begin, 1);
	else if ((*s == '>' || *s == '<') && f->quote == 0)
	{
		if (f->redir == 0 && *(s - 1) != ' ')
			(*cnt)++;
		f->redir++;
	}
	else if (*s != c && *s != '\''
				&& !(*s == '>' || *s == '<')
				&& f->quote == 0
				&& (*(s - 1) == '>' || *(s - 1) == '<'))
		get_cnt(&f->redir, cnt, &f->begin, 0);
	else if (*s == '\'' && f->quote == '\''
				&& (*(s - 1) == '>' || *(s - 1) == '<'))
		get_cnt(&f->redir, cnt, &f->begin, 0);
	else if (*s == '"' && f->quote == '"'
				&& (*(s - 1) == '>' || *(s - 1) == '<'))
		get_cnt(&f->redir, cnt, &f->begin, 0);
}

static int			ft_split_cnt(char const *s, const char c)
{
	int				cnt;
	t_split_flag	f;

	cnt = 0;
	f.begin = 0;
	f.quote = 0;
	f.redir = 0;
	while (*s)
	{
		put_quote_flag(s, &f.quote);
		if (f.begin == 0)
		{
			if (*s != c)
				f.begin = 1;
			if (*s == '<' || *s == '>')
				f.redir++;
		}
		else
			condition_to_cnt(&f, s, c, &cnt);
		s++;
	}
	return (f.begin == 0 ? cnt : cnt + 1);
}

int					word_redirect_len(char const **s, int *len)
{
	if ((**s) == '>' || (**s) == '<')
	{
		while (**s)
		{
			if ((**s) == '>' || (**s) == '<')
				(*len)++;
			else
				return (*len);
			(*s)++;
		}
	}
	return (*len);
}

int					condition_to_len_for_word(char const *s,
						const char c, int *len, t_split_flag *f)
{
	if (f->begin == 0)
	{
		if (*s != c)
		{
			f->begin = 1;
			(*len)++;
		}
	}
	else
	{
		if (*s == c && f->quote == 0)
			return (1);
		else if ((*s == '>' || *s == '<') && f->quote == 0)
			return (1);
		else
			(*len)++;
	}
	return (0);
}

static int			ft_word_len(char const *s, const char c)
{
	int				len;
	t_split_flag	f;

	len = 0;
	f.begin = 0;
	f.quote = 0;
	if (word_redirect_len(&s, &len) != 0)
		return (len);
	while (*s)
	{
		put_quote_flag(s, &f.quote);
		if(condition_to_len_for_word(s, c, &len, &f) == 1)
			break;
		s++;
	}
	return (len);
}

static char			*ft_alloc_word(int *idx, char const *s, const char c)
{
	int				word_len;
	char			*word;
	int				i;

	word_len = ft_word_len(&s[*idx], c);
	word = (char*)malloc(sizeof(char) + (word_len + 1));
	if (word == NULL)
		return (NULL);
	i = 0;
	while (i < word_len)
	{
		word[i] = s[*idx + i];
		i++;
	}
	word[i] = '\0';
	*idx += word_len;
	return (word);
}

static void			*ft_free(t_token *result, int len)
{
	int				i;

	i = 0;
	while (i < len)
		free(result[i++].cmd);
	return (NULL);
}

t_token				*cmd_split(char const *s, char c)
{
	int				i;
	int				cursor;
	t_token			*result;

	if (s == NULL)
		return (NULL);
	i = 0;
	cursor = 0;
	result = (t_token*)malloc(sizeof(t_token) * (ft_split_cnt(s, c) + 1));
	if (result == NULL)
		return (NULL);
	while (s[cursor])
		if (s[cursor] == c)
			cursor++;
		else
		{
			result[i].cmd = ft_alloc_word(&cursor, s, c);
			result[i].redir_flag = 0;
			if (result[i].cmd == NULL)
				return (ft_free(result, i));
			i++;
		}
	result[i].cmd = 0;
	return (result);
}
