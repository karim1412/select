/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keycode_handler.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kacoulib <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/14 04:48:24 by kacoulib          #+#    #+#             */
/*   Updated: 2017/07/14 04:48:36 by kacoulib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_select.h"

int					display_result(t_select select[], t_term_info *term_info)
{
	int				i;
	char			*tmp;

	if (!term_info->nb_select)
		return (0);
	if (!(tmp = tgetstr("cm", 0)))
		return (FALSE);
	tputs(tgoto(tmp, 0, term_info->select_len + 1), 0, tputs_display_function);
	i = -1;
	while (++i < term_info->select_len + 1)
	{
		if (select[i].is_select)
		{
			ft_putstr(select[i].content);
			if (--(term_info->nb_select) > 0)
				ft_putstr(" ");
		}
	}
	ft_putchar('\n');
	return (1);
}

int					keycode_space(t_select select[], t_term_info *t_info)
{
	if (select[t_info->y_pos].is_select && !(select[t_info->y_pos].is_select = FALSE))
		t_info->nb_select--;
	else if ((select[t_info->y_pos].is_select = TRUE))
		t_info->nb_select++;
	move_down(select, t_info);
	return (1);
}

int					keycode_delete(t_select select[], t_term_info *t_info)
{
	char			*tmp;

	if (!(tmp = tgetstr("cl", 0)))
		return (FALSE);
	tputs(tmp, 0, tputs_display_function);
	// tmp = tgetstr("cm", 0);
	// tputs(tgoto(tmp, 0, t_info->index), 0, tputs_display_function);
	// tmp = tgetstr("dl", 0);
	// tputs(tgoto(tmp, 0, t_info->index), 0, tputs_display_function);
	select[t_info->index].is_show = FALSE;
	if (select[t_info->index].is_select)
	{
		select[t_info->index].is_select = FALSE;
		t_info->nb_select--;
	}
	display_all_elem(select, t_info);

	if (t_info->nb_deleted++ == t_info->select_len)
		return (FALSE);
	return (move_down(select, t_info));
}

int					keyboard_events(char keycode[], t_select select[], t_term_info *t_info)
{
	if (!keycode)
		return (FALSE);
	if (keycode[0] == 27)
	{
		if (keycode[2] == 66)
			return (move_down(select, t_info));
		else if (keycode[2] == 65)
			return (move_up(select, t_info));
		else
			return (0);
	}
	else if (keycode[0] == 127)
		return(keycode_delete(select, t_info));
	else if (keycode[0] == 32)
		keycode_space(select, t_info);
	else if (keycode[0] == 13 || keycode[0] == 10)
		return (0);
	return (1);
}
