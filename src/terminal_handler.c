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

struct	termios		*get_terminal(void)
{
	static	struct termios *term = NULL;

	if (!term)
		term = malloc(sizeof(struct termios) + 1);
	return (term);
}

int					resize_handle(t_term_info *t_info)
{

	struct winsize 	w;


	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) != 0)
		return (FALSE);
	t_info->height = w.ws_row - 2;
	t_info->width = w.ws_col - 2;
	if (t_info->select_len > w.ws_row)
	{
		t_info->nb_col = w.ws_col / t_info->col_space;
		if ((t_info->select_len / t_info->nb_col) > w.ws_row)
		{
			ft_putendl("Sorry but the screen is too small");
		}
	}
	return (TRUE);
}

/*
**
**
** @return return the top head of created term_info
*/

t_term_info			*get_or_init_term(char **av, struct termios	*term)
{
	int				i;
	int				tmp;
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
	new_term_info->index = 0;
	new_term_info->last_pos = 0;
	new_term_info->nb_select = 0;
	new_term_info->select_len = arr_len(av) - 1;
	new_term_info->x_pos = 0;
	new_term_info->y_pos = 0;
	new_term_info->col_space = tmp + 3;
	new_term_info->nb_col = 0;
	new_term_info->term = term;
	if (!resize_handle(new_term_info))
		return (FALSE);
	return (new_term_info);
}

int					reset_term(struct termios *term)
{
	char					*tmp;

	if (tcgetattr(0, term) == -1)
		return (FALSE);
	term->c_lflag = (ICANON | ECHO | ISIG);
	term->c_cc[VMIN] = 1;
	term->c_cc[VTIME] = 100;
	if (!(tmp = tgetstr("te", 0)))
		return (FALSE);
	tputs(tmp, 0, tputs_display_function);
	if (!(tmp = tgetstr("ve", 0)))
		return (FALSE);
	tputs(tmp, 0, tputs_display_function);
	if (!(tmp = tgetstr("se", 0))) 
		return (FALSE);
	tputs(tmp, 0, tputs_display_function);
	if (!(tmp = tgetstr("ue", 0)))
		return (FALSE);
	tputs(tmp, 0, tputs_display_function);
	if (tcsetattr(0, TCSADRAIN, term) < 0)
		return (FALSE);
	return (TRUE);
}

int					set_cannic_mode(struct termios *term)
{
	char					*term_name;
	char					*tmp;

	tmp = NULL;
	if (!(term_name = getenv("TERM")))
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
