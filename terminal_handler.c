/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminal_handler.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kacoulib <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/22 19:21:25 by kacoulib          #+#    #+#             */
/*   Updated: 2017/07/22 19:21:51 by kacoulib         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_select.h"

struct	termios				*get_terminal(void)
{
	static	struct termios	*term = NULL;

	if (!term)
		term = malloc(sizeof(struct termios) + 1);
	return (term);
}

static void					init_term_last_part(t_term_info *new_term_info)
{
	new_term_info->index = 0;
	new_term_info->last_pos = 0;
	new_term_info->nb_select = 0;
	new_term_info->nb_deleted = 0;
	new_term_info->x_pos = 0;
	new_term_info->y_pos = 0;
	new_term_info->nb_col = 0;
	new_term_info->is_win_size_ok = FALSE;
}

/*
**	Set the terminal if not get it
**
** @return return the top head of created term_info
*/

t_term_info					*get_or_init_term(char **av, struct termios	*term)
{
	int						i;
	int						tmp;
	static t_term_info		*new_term_info = NULL;

	if (new_term_info)
		return (new_term_info);
	tmp = 0;
	i = -1;
	while (av[++i])
		if ((int)ft_strlen(av[i]) > tmp)
			tmp = ft_strlen(av[i]);
	if (!(new_term_info = malloc(sizeof(t_term_info) + 1)))
		return (NULL);
	new_term_info->select_len = arr_len(av) - 1;
	new_term_info->col_space = tmp + 1;
	new_term_info->term = term;
	init_term_last_part(new_term_info);
	update_screen_info();
	return (new_term_info);
}

int							reset_term(struct termios *term)
{
	char					*tmp;

	if (tcgetattr(0, term) == -1)
		return (FALSE);
	term->c_lflag = (ICANON | ECHO | ISIG);
	term->c_cc[VMIN] = 1;
	term->c_cc[VTIME] = 100;
	if (!(tmp = tgetstr("te", 0)))
		return (FALSE);
	tputs(tmp, 1, tputs_display_function);
	if (!(tmp = tgetstr("ve", 0)))
		return (FALSE);
	tputs(tmp, 1, tputs_display_function);
	if (!(tmp = tgetstr("se", 0)))
		return (FALSE);
	tputs(tmp, 1, tputs_display_function);
	if (!(tmp = tgetstr("ue", 0)))
		return (FALSE);
	tputs(tmp, 1, tputs_display_function);
	if (tcsetattr(0, TCSADRAIN, term) < 0)
		return (FALSE);
	return (TRUE);
}

int							set_cannic_mode(struct termios *term)
{
	char					*term_name;
	char					*tmp;

	tmp = NULL;
	if (!(term_name = getenv("TERM")))
		return (FALSE);
	if (ft_strcmp(term_name, "xterm-256color") != 0)
		return (FALSE);
	if (tgetent(0, term_name) < 1)
		return (FALSE);
	if (tcgetattr(0, term) < 0)
		return (FALSE);
	term->c_lflag &= ~(ICANON | ECHO);
	if (tcsetattr(0, TCSADRAIN, term) < 0)
		return (FALSE);
	return (TRUE);
}
